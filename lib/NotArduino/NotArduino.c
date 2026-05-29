#include "NotArduino.h"

static PORT_Type *PORTS[] = {PORTA, PORTB, PORTC, PORTD, PORTE};
static GPIO_Type *GPIOS[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE};

void pinMode(NotArduinoPin pin, NotArduinoPinMode mode)
{
    int port = pin / 32;
    int pinInPort = pin % 32;

    PORTS[port]->PCR[pinInPort] |= 0b001 << 8; // Mux to GPIO

    (mode == OUTPUT) ? (GPIOS[port]->PDDR |= (1 << pinInPort)) : (GPIOS[port]->PDDR &= ~(1 << pinInPort));
}

void digitalWrite(NotArduinoPin pin, NotArduinoDigitalValue digitalValue)
{
    int port = pin / 32;
    int pinInPort = pin % 32;

    (digitalValue == HIGH) ? (GPIOS[port]->PDOR |= (1 << pinInPort)) : (GPIOS[port]->PDOR &= ~(1 << pinInPort));
}

NotArduinoDigitalValue digitalRead(NotArduinoPin pin)
{
    int port = pin / 32;
    int pinInPort = pin % 32;

    return (GPIOS[port]->PDIR & (1 << pinInPort)) ? HIGH : LOW;
}

typedef struct
{
    uint8_t valid;
    TPM_Type *tpm;
    uint8_t ch;
    uint8_t alt;
} TpmPinConfig;

static uint8_t pinInited[160];
static uint8_t tpmInited[3];
static uint8_t chInited[3][6];

// Returns predefined TPM config for a given pin, or valid=0 if pin doesn't have predefined TPM functionality
static TpmPinConfig TpmConfigFromPin(NotArduinoPin pin)
{
    switch (pin)
    {
    case PB18:
        return (TpmPinConfig){1, TPM2, 0, 3};
    case PB19:
        return (TpmPinConfig){1, TPM2, 1, 3};
    case PD1:
        return (TpmPinConfig){1, TPM0, 1, 4};
    default:
        return (TpmPinConfig){0, TPM0, 0, 0}; // Invalid config
    }
}

/**
 * Initializes the pin for TPM use if it hasn't been initialized yet. Sets the correct MUX mode for the pin, enables clock gating for the PORT, and disables GPIO output driver to avoid conflicts with TPM functionality.
 */
static void pinInitOnce(NotArduinoPin pin, uint8_t alt)
{
    int port = pin / 32;
    int pinInPort = pin % 32;
    // Enable clock to the PORT
    uint32_t portMasks[] = {
        SIM_SCGC5_PORTA_MASK,
        SIM_SCGC5_PORTB_MASK,
        SIM_SCGC5_PORTC_MASK,
        SIM_SCGC5_PORTD_MASK,
        SIM_SCGC5_PORTE_MASK};
    SIM->SCGC5 |= portMasks[port];

    // Disable GPIO output driver (clear PDDR bit) to avoid conflict with TPM
    GPIOS[port]->PDDR &= ~(1 << pinInPort);

    // Clear ALT bits (10:8) and set to correct TPM mode
    PORTS[port]->PCR[pinInPort] = (PORTS[port]->PCR[pinInPort] & ~(0x7 << 8)) | (alt << 8);
}

/**
 * Initializes the TPM module if it hasn't been initialized yet. Enables clock gating for the TPM module, selects the clock source, and sets the prescaler and MOD for ~10kHz PWM frequency. Does NOT enable the counter yet, as that should only be done after all channels are configured to avoid unintended behavior.
 */
static void tpmInitOnce(TPM_Type *tpm)
{
    int i = (tpm == TPM0) ? 0 : (tpm == TPM1) ? 1
                                              : 2;
    if (i < 0 || i > 2 || tpmInited[i])
        return;

    // Enable clock gating for TPM module
    if (tpm == TPM0)
    {
        SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
    }
    else if (tpm == TPM1)
    {
        SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
    }
    else if (tpm == TPM2)
    {
        SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;
    }

    // Select clock source for TPM (MCGFLLCLK or MCGPLLCLK)
    SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

    // Set prescaler and MOD, but DON'T enable counter yet
    tpm->SC = TPM_SC_PS(0);
    tpm->MOD = 4799; // ~10kHz PWM frequency

    tpmInited[i] = 1; // Mark this TPM as initialized
}

/**
 * Initializes a TPM channel if it hasn't been initialized yet. Sets the correct configuration for the channel, including the PWM mode and initial duty cycle.
 */
static void tpmChannelInitOnce(TPM_Type *tpm, uint8_t ch)
{
    int i = (tpm == TPM0) ? 0 : (tpm == TPM1) ? 1
                                              : 2;
    if (i < 0 || i > 2 || ch > 5 || chInited[i][ch])
        return;

    tpm->CONTROLS[ch].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK; // Edge-aligned PWM, High-true pulses
    tpm->CONTROLS[ch].CnV = 0;                                       // Start with 0% duty cycle

    chInited[i][ch] = 1; // Mark this channel as initialized
}

void analogWrite(NotArduinoPin pin, uint8_t value)
{
    TpmPinConfig cfg = TpmConfigFromPin(pin);
    if (!(cfg.valid))
        return; // Invalid pin for TPM

    // Stop the counter before configuring/reconfiguring a channel
    cfg.tpm->SC &= ~TPM_SC_CMOD_MASK;

    if (!pinInited[pin])
    {
        pinInitOnce(pin, cfg.alt);
        tpmInitOnce(cfg.tpm);
        tpmChannelInitOnce(cfg.tpm, cfg.ch);

        pinInited[pin] = 1;
    }

    uint32_t mod = cfg.tpm->MOD; // So it can be changed at runtime
    // Scale 0-255 to 0-MOD with proper rounding
    cfg.tpm->CONTROLS[cfg.ch].CnV = (mod * (uint32_t)value + 127) / 255;

    // Re-enable TPM counter after configuration is complete
    cfg.tpm->SC |= TPM_SC_CMOD(1);
}
//MAP
int map(int x, int in_min, int in_max, int out_min, int out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}