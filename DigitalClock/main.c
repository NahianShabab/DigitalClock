/*
 * DigitalClock.c
 *
 * Created: 03-Sep-22 5:03:45 PM
 * Author : Nahian
 */ 

#include <avr/io.h>
#define F_CPU 1000000
#include<util/delay.h>
#include<avr/interrupt.h>
#define SEGMENT_DELAY 1000 //in microsecond
#define MAX_TIME 86400000000 // number of microseconds in 1 day or 24 hour

volatile uint64_t time=19260000000+24*60*1000000; //microsecond
void initTimer(){
	TCCR1A=0b00000000;
	TCCR1B=0b00000001;
	TIMSK=0b00000100;
	sei();
}

ISR(TIMER1_OVF_vect){
	time=(time+65536)%MAX_TIME;
}

char segmentMap[10]={
	0b11000000, //0
	0b01110011, //1
	0b00100100, //2
	0b00100001, //3
	0b00010011, //4
	0b00001001, //5
	0b00001000, //6
	0b01100011, //7
	0b00000000, //8
	0b00000001, //9
};

int main(void)
{
    /* Replace with your application code */
	DDRB=0xff;
	DDRD=0xff;
	PORTD=0;
	initTimer();
    while (1) 
    {
		int minute,hour,second;
		hour=(time/(((uint64_t)1000000)*3600));
		PORTB=segmentMap[hour/10];
		PORTD=1;
		_delay_us(SEGMENT_DELAY);
		PORTB=segmentMap[hour%10];
		PORTD=2;
		_delay_us(SEGMENT_DELAY);
		minute=(time/(1000000*60))%60;
		PORTB=segmentMap[minute/10];
		PORTD=4;
		_delay_us(SEGMENT_DELAY);
		PORTB=segmentMap[minute%10];
		PORTD=8;
		_delay_us(SEGMENT_DELAY);
		second=(time/1000000)%60;
		PORTB=segmentMap[second/10];
		PORTD=16;
		_delay_us(SEGMENT_DELAY);
		PORTB=segmentMap[second%10];
		PORTD=32;
		_delay_us(SEGMENT_DELAY);
    }
}

