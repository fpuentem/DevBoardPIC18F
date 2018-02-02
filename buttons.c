#include "buttons.h"
#include <stdint.h>
#include <xc.h>

#define BUTTON1 PORTAbits.RA3
#define BUTTON2 PORTAbits.RA4

typedef union{
	uint8_t buttons;
	struct {
		uint8_t button0:1;
		uint8_t button1:1;
		uint8_t button2:1;
	        uint8_t button3:1;
		uint8_t button4:1;
		uint8_t button5:1;
		uint8_t button6:1;
		uint8_t button7:1;	
	};
} buttons_t;

buttons_t filtered_buttons;
buttons_t last_buttons;

inline void Buttons_Init(){
	filtered_buttons.buttons = 0;
	last_buttons.buttons = 0;
	TRISA4 = 1;
	TRISA3 = 1;
}

inline void task_Buttons(){
	buttons_t raw_buttons;

	if(!ButtonsDelay){
		ButtonsDelay = 15;
		raw_buttons.button1 = !BUTTON1;
		raw_buttons.button2 = !BUTTON2;

		filtered_buttons.buttons = raw_buttons.buttons & last_buttons.buttons;
		last_buttons.buttons = raw_buttons.buttons;
	}else{
		ButtonsDelay--;
	}


}
