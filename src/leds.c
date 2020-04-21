
#include "main.h"
#include "leds.h"


void SetLED(void) {
    CLEAR_BIT(LED_TRIS, LED_PIN);
    SET_BIT(LED_PORT, LED_PIN);
}

void ResetLED(void) {
    CLEAR_BIT(LED_TRIS, LED_PIN);
    CLEAR_BIT(LED_PORT, LED_PIN);
}

void ToggleLED(void) {
    CLEAR_BIT(LED_TRIS, LED_PIN);
    TOGGLE_BIT(LED_PORT, LED_PIN);
}
