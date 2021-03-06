/*
* Sensors.c
* 
* Created: 16/4/2014 around 13:30 PM
* Author: Martin
*/

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>
#include "sensors.h"
#include "display.h"

static uint8_t gSelectedSensor = 0;
static uint8_t gSensorBuffer[8];

static float IR_short[13][2];
static float IR_long[15][2];
static bool sensor_data_flag = false;

static void select_sensor(int sensor);
static void start_ul_sensor();
static int voltage_to_mm_short(float voltage);
static int voltage_to_mm_long(float voltage);
static void adc_init();
static void adc_start();
static void init_mux();
static void init_tables();
static void init_UL();

void sensors_init()
{
	adc_init();
	init_tables();
	init_mux();
	init_UL();
}

void init_tables()
{
	// 10-80 cm
	IR_short[0][0] = 3.15;
	IR_short[0][1] = 6;
	
	IR_short[1][0] = 2.98;
	IR_short[1][1] = 7;

	IR_short[2][0] = 2.75;
	IR_short[2][1] = 8;
	
	IR_short[3][0] = 2.31;
	IR_short[3][1] = 10;
	
	IR_short[4][0] = 1.64;
	IR_short[4][1] = 15;
	
	IR_short[5][0] = 1.31;
	IR_short[5][1] = 20;
	
	IR_short[6][0] = 1.08;
	IR_short[6][1] = 25;
	
	IR_short[7][0] = 0.92;
	IR_short[7][1] = 30;
	
	IR_short[8][0] = 0.74;
	IR_short[8][1] = 40;
	
	IR_short[9][0] = 0.61;
	IR_short[9][1] = 50;
	
	IR_short[10][0] = 0.51;
	IR_short[10][1] = 60;
	
	IR_short[11][0] = 0.45;
	IR_short[11][1] = 70;
	
	IR_short[12][0] = 0.41;
	IR_short[12][1] = 80;
	
	// 20-150 cm
	IR_long[0][0] = 2.75;
	IR_long[0][1] = 15;
	
	IR_long[1][0] = 2.55;
	IR_long[1][1] = 20;
	
	IR_long[2][0] = 2.00;
	IR_long[2][1] = 30;
	
	IR_long[3][0] = 1.55;
	IR_long[3][1] = 40;
	
	IR_long[4][0] = 1.25;
	IR_long[4][1] = 50;
	
	IR_long[5][0] = 1.07;
	IR_long[5][1] = 60;
	
	IR_long[6][0] = 0.85;
	IR_long[6][1] = 70;
	
	IR_long[7][0] = 0.80;
	IR_long[7][1] = 80;
	
	IR_long[8][0] = 0.75;
	IR_long[8][1] = 90;
	
	IR_long[9][0] = 0.65;
	IR_long[9][1] = 100;
	
	IR_long[10][0] = 0.60;
	IR_long[10][1] = 110;
	
	IR_long[11][0] = 0.55;
	IR_long[11][1] = 120;
	
	IR_long[12][0] = 0.50;
	IR_long[12][1] = 130;
	
	IR_long[13][0] = 0.45;
	IR_long[13][1] = 140;
	
	IR_long[14][0] = 0.42;
	IR_long[14][1] = 150;
}

int voltage_to_mm_short(float voltage)
{
	if(voltage >= IR_short[0][0])
	{
		return IR_short[0][1]*10;
	} else if(voltage <= IR_short[12][0])
	{
		return IR_short[12][1]*10;
	}
	
	for(int i = 0; i < 13; ++i)
	{
		float prev = IR_short[i][0];
		float next = IR_short[i+1][0];
		if(next == voltage)
		{
			return IR_short[i+1][1]*10;
		} else if(prev > voltage && next < voltage)
		{
			int high = IR_short[i][1]*10;
			int low = IR_short[i+1][1]*10;
			int diff = high - low;
			float diff_to_prev = prev - voltage;
			float volt_diff = prev - next;
			return (int) (high - diff * diff_to_prev / volt_diff);
		}
	}
	
	return 0;
}

int voltage_to_mm_long(float voltage)
{
	if(voltage >= IR_long[0][0])
	{
		return IR_long[0][1]*10;
	} else if(voltage <= IR_long[14][0])
	{
		return IR_long[14][1]*10;
	}
	
	for(int i = 0; i < 13; ++i)
	{
		float prev = IR_long[i][0];
		float next = IR_long[i+1][0];
		if(next == voltage)
		{
			return IR_long[i+1][1]*10;
		} else if(prev > voltage && next < voltage)
		{
			int high = IR_long[i][1]*10;
			int low = IR_long[i+1][1]*10;
			int diff = high - low;
			float diff_to_prev = prev - voltage;
			float volt_diff = prev - next;
			return (int) (high - diff * diff_to_prev / volt_diff);
		}
	}
	
	return 0;
}void init_mux()
{
	DDRA |= 0b00111110;
	DDRA &= ~(1<<PORTA0);
	PORTA &= ~(1<<PORTA5);
	PORTA &= ~((1<<PORTA1) | (1<<PORTA2) | (1<<PORTA3) | (1<<PORTA4));
	//PORTA &= 0b11100001;
}

void init_UL()
{
	DDRD |= 1;
	PCICR = 1;
	PCMSK0 = (1<<PCINT6);
	DDRA |= (1<<PORTA7);
	DDRA &= ~(1<<PORTA6);
	TCCR0B = 0x05;
}

void adc_init()
{
	// ADC enabled, enable interrupt, set division factor for clock to be 128
	ADCSRA = (1<<ADEN | 1<<ADIE | 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0);
	
	// Left adjust, set voltage reference selection
	ADMUX = 1<<ADLAR | 1<<REFS0;
}

void adc_start()
{
	ADCSRA |= 1<<ADSC;
}

void sensors_start_sample()
{
	select_sensor(0);
	adc_start();
}

void sensors_display_data()
{
	display_clear();
	
	display_set_pos(0,0);
	display_text("0: ");
	display_value(gSensorBuffer[0]);
	
	display_set_pos(0,8);
	display_text("1: ");
	display_value(gSensorBuffer[1]);
	
	display_set_pos(1,0);
	display_text("2: ");
	display_value(gSensorBuffer[2]);
	
	display_set_pos(1,8);
	display_text("3: ");
	display_value(gSensorBuffer[3]);
	
	display_set_pos(2,0);
	display_text("4: ");
	display_value(gSensorBuffer[4]);
	
	display_set_pos(2,8);
	display_text("5: ");
	display_value(gSensorBuffer[5]);
	
	display_set_pos(3,0);
	display_text("6: ");
	display_value(gSensorBuffer[6]);
	
	display_set_pos(3,8);
	display_text("7: ");
	display_value(gSensorBuffer[7]);
}

void select_sensor(int sensor)
{
	gSelectedSensor = sensor;
	PORTA &= ~((1<<PORTA1) | (1<<PORTA2) | (1<<PORTA3) | (1<<PORTA4));
	switch(sensor)
	{
		case(0):
		PORTA |= 1<<PORTA1;
		break;
		case(1):
		PORTA |= 1<<PORTA2;
		break;
		case(2):
		PORTA |= 1<<PORTA1 | 1<<PORTA2;
		break;
		case(3):
		PORTA |= 1<<PORTA3;
		break;
		case(4):
		PORTA |= 1<<PORTA1 | 1<<PORTA3;
		break;
		case(5):
		PORTA |= 1<<PORTA2 | 1<<PORTA3;
		break;
		case(6):
		PORTA |= 1<<PORTA1 | 1<<PORTA2 | 1<<PORTA3;
		break;
		default:
		//Do nada
		break;
	}
}

void start_ul_sensor()
{
	TCNT0 = 0;
	PORTA |= (1<<PORTA7);
	_delay_us(15);
	PORTA &= ~(1<<PORTA7);
}

bool sensors_sampling_done()
{
	return sensor_data_flag;
}

void sensors_reset_flag()
{
	sensor_data_flag = false;
}


ISR(ADC_vect)
{
	cli();	uint8_t adcValue = ADCH;	float vin = adcValue * 5.0 / 256.0;	if(gSelectedSensor == 4)	{		gSensorBuffer[gSelectedSensor] = voltage_to_mm_long(vin)/10;		} else {		gSensorBuffer[gSelectedSensor] = voltage_to_mm_short(vin)/10;	}		if(gSelectedSensor < 6)	{		// Not last sensor		select_sensor(gSelectedSensor + 1);		adc_start();	} else {
		select_sensor(0);		start_ul_sensor();	}	sei();}

ISR(PCINT0_vect)
{
	cli();
	if(PINA & (1<<PINA6))
	{
		TCNT0 = 0;
	}
	else
	{
		uint8_t UL = TCNT0;
		gSensorBuffer[7] = UL;
		sensor_data_flag = true;
	}
	sei();
}

