/*	Author: bzhan063
 *  Partner(s) Name: Richard Manzano & Kyle Garcia
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "io.h"
#include "simAVRHeader.h"
#include "timer.h"
#include "bit.h"
#ifdef _SIMULATE_

#endif

unsigned char GetKeypadKey(){
	PORTC = 0XEF;
	asm("nop");
	if(GetBit(PINC,0) == 0) { return('1');}
	if(GetBit(PINC,1) == 0) { return('4');}
	if(GetBit(PINC,2) == 0) { return('7');}
	if(GetBit(PINC,3) == 0) { return('*');}

	PORTC = 0XDF;
	asm("nop");
	if(GetBit(PINC,0) == 0) { return('2');}
	if(GetBit(PINC,1) == 0) { return('5');}
	if(GetBit(PINC,2) == 0) { return('8');}
	if(GetBit(PINC,3) == 0) { return('0');}

	PORTC = 0XBF;
	asm("nop");
	if(GetBit(PINC,0) == 0) { return('3');}
	if(GetBit(PINC,1) == 0) { return('6');}
	if(GetBit(PINC,2) == 0) { return('9');}
	if(GetBit(PINC,3) == 0) { return('#');}

	PORTC = 0X7F;
	asm("nop");
	if(GetBit(PINC,0) == 0) { return('A');}
	if(GetBit(PINC,1) == 0) { return('B');}
	if(GetBit(PINC,2) == 0) { return('C');}
	if(GetBit(PINC,3) == 0) { return('D');}

	return('\0');
}

typedef struct _task {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct)(int);
} task;

// Shared Variables
unsigned char led0_output = 0x00;
unsigned char led1_output = 0x00;
unsigned char pause = 0;
// end

enum pauseButtonSM_States { pauseButton_wait, pauseButton_press, pauseButton_release };

int pauseButtonSMTick (int state){
	
	unsigned char press = ~PINA & 0x01;
	switch (state) {
		case pauseButton_wait:
			state = press == 0x01? pauseButton_press: pauseButton_wait;
			break;
		case pauseButton_press:
			state = pauseButton_release;
			break;
		case pauseButton_release:
			state = press == 0x00? pauseButton_wait: pauseButton_press;
		default:
			break;
	}
	switch (state) {
		case pauseButton_wait:
			break;
		case pauseButton_press:
			pause = (pause == 0) ? 1 : 0;
			break;
		case pauseButton_release:	
			break;
		default:
			break;
	}
	
	return state;
}

enum toggleLED0_States { toggleLED0_wait, toggleLED0_blink } ;

int toggleLED0SMTick (int state){
	switch (state) {
		case toggleLED0_wait:
			state = !pause? toggleLED0_blink: toggleLED0_wait;
			break;
		case toggleLED0_blink:
			state = pause? toggleLED0_wait: toggleLED0_blink;
			break;
		default:
			state = toggleLED0_wait;
			break;
	}
	switch (state) {
		case toggleLED0_wait:
			break;
		case toggleLED0_blink:
			led0_output = (led0_output == 0x00) ? 0x01 : 0x00;
			break;
		default:
			break;
	}
	return state;
}

enum toggleLED1_States { toggleLED1_wait, toggleLED1_blink } ;


int toggleLED1SMTick (int state){
	switch (state) {
		case toggleLED1_wait:
			state = !pause? toggleLED1_blink: toggleLED1_wait;
			break;
		case toggleLED1_blink:
			state = pause? toggleLED1_wait: toggleLED1_blink;
			break;
		default:
			state = toggleLED1_wait;
			break;
	}
	switch (state) {
		case toggleLED1_wait:
			break;
		case toggleLED1_blink:
			led1_output = (led1_output == 0x00) ? 0x01 : 0x00;
			break;
		default:
			break;
	}
	return state;
}

enum display_States { display_display };

int displaySMTick (int state) {
	
	unsigned char output;
	switch (state) {
		case display_display: 
			state = display_display; 
			break;
		default:
			state = display_display;
			break;
	}
	switch(state) {
		case display_display:
			output = led0_output | led0_output << 1;
			break;
		default:
			break;
	}
	PORTB = output;
	return state;
}

unsigned long int findGCD(unsigned long int a, unsigned long int b) {

	unsigned long int c;
	while(1){
		c = a%b;
		if( c == 0){
			return b;
		}
		a = b;
		b = c;
	}
	return 0;
}



int main(void){//SM based main function
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	static task task1, task2, task3, task4;
	task *tasks[] = { &task1, &task2, &task3, &task4};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	
	task1.state = 0x00;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &pauseButtonSMTick;
	
	task2.state = 0x00;
	task2.period = 500;
	task2.elapsedTime = task2.period;
	task2.TickFct = &toggleLED0SMTick;

	task3.state = 0x00;
	task3.period = 1000;
	task3.elapsedTime = task3.period;
	task3.TickFct = &toggleLED1SMTick;
	
	task4.state = 0x00;
	task4.period = 1000;
	task4.elapsedTime = task4.period;
	task4.TickFct = &displaySMTick;

	unsigned short i;
	unsigned long GCD = tasks[0]->period;
	for ( i = 1; i < numTasks; i++ ){
		GCD = findGCD(GCD, tasks[i]->period);
	}

	TimerSet(GCD);
	TimerOn();
	
	
	while(1){
		
		for(i = 0; i < numTasks; i++){
			if(tasks[i]->elapsedTime == tasks[i]->period){
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += GCD;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}



