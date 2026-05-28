#include "NotArduino.h"

void main(void) {
    pinMode(PB18, OUTPUT);
    // digitalWrite(PB18, LOW);

    pinMode(PB19, OUTPUT);
    // digitalWrite(PB19, LOW);

    pinMode(PA4, INPUT);
    while (1) {
        if (digitalRead(PA4) == LOW) {
            digitalWrite(PB18, HIGH);
            digitalWrite(PB19, LOW);
        } else {
            digitalWrite(PB18, LOW);
            digitalWrite(PB19, HIGH);
        }
    }
}