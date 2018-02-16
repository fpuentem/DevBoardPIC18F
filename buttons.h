#ifndef BUTTONS_H
#define BUTTONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include<stdint.h>

	typedef union{
		uint8_t buttons;

		struct{
			uint8_t button0 : 1;
			uint8_t button1 : 1;
			uint8_t button2 : 1;
			uint8_t button3 : 1;
			uint8_t button4 : 1;
			uint8_t button5 : 1;
			uint8_t button6 : 1;
			uint8_t button7 : 1;
		};
	} buttons_t;

	extern buttons_t filtered_buttons;

#define BUTTON1 PORTAbits.RA3
#define BUTTON2 PORTAbits.RA4
	extern volatile uint8_t ButtonsDelay;
	extern buttons_t last_buttons;

	inline void Buttons_Init();
	inline void task_Buttons();

#ifdef	__cplusplus
}
#endif

#endif /* BUTTONS_H */
