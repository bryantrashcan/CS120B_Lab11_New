

/*	Author: rmanz004
 *  Partner(s) Name: Kyle Garcia and Bryant Zhang
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <io.h>
#include <bit.h>
#include <timer.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
typedef struct _task {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct) (int);
} task;

/*
unsigned char led0_output = 0x00;
unsigned char led1_output = 0x00;
unsigned char pause = 0;
enum pauseButtonSM_States {pauseButton_wait, pauseButton_press, pauseButton_release } ;
enum toggleLED0_States { toggleLED0_wait, toggleLED0_blink } ;
enum toggleLED1_States { toggleLED1_wait, toggleLED1_blink } ;
enum display_States { display_display };
int pauseButtonSMTick (int state){
	
	usigned char press = ~PINA & 0x01;
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
	PORTD = output;
	return state;
}
*/

unsigned char GetKeypadKey() {

	PORTC = 0xEF;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('1'); }
	if (GetBit(PINC,1)==0) { return('4'); }
	if (GetBit(PINC,2)==0) { return('7'); }
	if (GetBit(PINC,3)==0) { return('*'); }

	PORTC = 0xDF;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('2'); }
	if (GetBit(PINC,1)==0) { return('5'); }
	if (GetBit(PINC,2)==0) { return('8'); }
	if (GetBit(PINC,3)==0) { return('0'); }

	PORTC = 0xBF;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('3'); }
	if (GetBit(PINC,1)==0) { return('6'); }
	if (GetBit(PINC,2)==0) { return('9'); }
	if (GetBit(PINC,3)==0) { return('#'); }

	PORTC = 0x7F;
	asm("nop");
	if (GetBit(PINC,0)==0) { return('A'); }
	if (GetBit(PINC,1)==0) { return('B'); }
	if (GetBit(PINC,2)==0) { return('C'); }
	if (GetBit(PINC,3)==0) { return('D'); }
		
	return('\0');
}

/*
int main(void) { // Non-SM based main function
    	DDRA = 0x00; PORTD = 0xFF;
	DDRB = 0xFF; PORTC = 0x00;
	static _task, task1, task2, task3, task4;
	_task *tasks[] {&task1, &task2, &task3, &task4 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	task1.state = start;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &pauseButtonToggleSMTick;
	task2.state = start;
	task2.period = 500;
	task2.elapsedTime = task2.period;
	task2.TickFct = &toggleLED0SMTick;
	task3.state = start;
	task3.period = 1000;
	task3.elapsedTime = task3.period;
	task3.TickFct = &toggleLED1SMTick;;
	task4.state = start;
	task4.period = 10;
	task4.elapsedTime = task4.period;
	task4.TickFct = &displaySMTick;
	TimerSet(0);//GCD val should go here
	TimerOn();
	unsigned short i;
	while (1) {
		for ( i = 0; i < numTasks; i+=) {
			if ( tasks[i]->elapsedTime == tasks[i]->period) {
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 0;//GCD val should go here
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}
*/

unsigned char tmp = 0x00;
unsigned char KeyNum = 0x00;

enum states{WAIT, GET_KEY_NUM} state;

int keypadTick(int state) {
	switch(state) { //Transitions 
		case WAIT:
			if(tmp == 0x1F){
				state = WAIT; 
  				break;
			}
			else if(tmp != 0x1F){
				state = GET_KEY_NUM; 
				break;
			}		
		case GET_KEY_NUM:
			state = WAIT; 
			break;
		default:
			state = WAIT;
			tmp = 0; 
			break;
	}
	switch (state) { // Actions
	
		case WAIT:
			tmp = GetKeypadKey();
			break;
		
		case GET_KEY_NUM:
			KeyNum = tmp;
			tmp = 0;
			break;
		
		default:
			break;
	}
	
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

/*
unsigned long GCD = tasks[0]->period;
for ( i = 1; i < numTasks; i++ ){
	GCD = findGCD(GCD, tasks[i]->period);
}
*/




int main(void){//SM based main function
	
	DDRD = 0xFF; PORTD = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	
	unsigned long int SMTick1_calc = 50;
	
	unsigned long int tmpGCD = 0;
	tmpGCD = findGCD(tmpGCD, SMTick1_calc);
	
	unsigned long int GCD = tmpGCD;
	
	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	
	static task task1;
	task *tasks[] = { &task1 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	task1.state = -1;
	task1.period = SMTick1_period;
	task1.elapsedTime = SMTick1_period;
	task1.TickFct = &keypadTick;
	
	TimerSet(GCD);
	TimerOn();
	
	unsigned short i;
	while(1) {
		for ( i = 0; i < numTasks; i++ ) {
			if ( tasks[i]->elapsedTime == tasks[i]->period ){
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		
		switch (KeyNum){
			case '\0': PORTD = 0x1F; break;
			case '1': PORTD = 0x01; break;
			case '2': PORTD = 0x02; break;
			case '3': PORTD = 0x03; break;
			case '4': PORTD = 0x04; break;
			case '5': PORTD = 0x05; break;
			case '6': PORTD = 0x06; break;
			case '7': PORTD = 0x07; break;
			case '8': PORTD = 0x08; break;
			case '9': PORTD = 0x09; break;
			case 'A': PORTD = 0x0A; break;
			case 'B': PORTD = 0x0B; break;
			case 'C': PORTD = 0x0C; break;
			case 'D': PORTD = 0x0D; break;
			case '*': PORTD = 0x0E; break;
			case '0': PORTD = 0x00; break;
			case '#': PORTD = 0x0F; break;
			default: PORTD = 0x1B; break;
			
			while (!TimerFlag);
			TimerFlag = 0;
		}
	}
}
