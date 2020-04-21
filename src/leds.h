/* 
 * File:   leds.h
 * Author: USUARIO
 *
 * Created on 26 de julio de 2016, 10:20 AM
 */

#ifndef LEDS_H
#define	LEDS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define LED_PIN    _BV(1)
#define LED_PORT   PORTC
#define LED_TRIS   TRISC


#define HERTZPIN    RC5
#define HERTZ_PIN   (1<<5)
#define HERTZ_PORT  PORTC
#define HERTZ_TRIS  TRISC



    void SetLED(void);

    void ResetLED(void);

    void ToggleLED(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LEDS_H */

