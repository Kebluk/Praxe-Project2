#include "NotArduino.h"
#include "MKL25Z4.h"

static PORT_Type * PORTS[] = {PORTA, PORTB, PORTC, PORTD, PORTE};
static GPIO_Type * GPIOS[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE};

void pinMode(NotArduinoPin pin, NotArduinoPinMode mode) {
    int port = pin / 32;
    int pinInPort = pin % 32;

    PORTS[port]->PCR[pinInPort] |= 0b001 << 8; // Mux to GPIO

    (mode == OUTPUT) ? (GPIOS[port]->PDDR |= (1 << pinInPort)) : (GPIOS[port]->PDDR &= ~(1 << pinInPort));
}

void digitalWrite(NotArduinoPin pin, NotArduinoDigitalValue digitalValue) {
    int port = pin / 32;
    int pinInPort = pin % 32;

    //(value)

}

int digitalRead(NotArduinoPin pin) {
    int port = pin / 32;
    int pinInPort = pin % 32;

}