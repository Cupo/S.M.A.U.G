/*
 * twi.c
 *
 * Created: 4/4/2014 2:40:42 PM
 *  Author: perjo018
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>
#include "twi.h"


// Global variables for response functions
int my_adress;
char message[255];
int message_counter;
int settings;
int buffer[7];
int sensors[7];
int servo;
int sweep;
int sensor;
int command[3];
int current_command;
int message_length;

//Global variables to exist in the module
bool instruction;
int current_instruction;
int my_adress;

void init_TWI(int module_adress)
{
	my_adress = module_adress;
	switch(module_adress)
	{
		case(C_ADRESS):
		{
			PORTC = 0x03; // Pull up, only 1!
			set_twi_reciever_enable();
			//TWSR = (0<<TWPS0) | (0<<TWPS1); //Prescaler 0 0 -> 1
			TWBR = 0b00011011; //bit rate 27 => clk = 79.448 kHz
			TWAR = (1<<TWA6) | (1<<TWGCE); // Address 100 0000, General Call Accepted
			break;
		}
		case(S_ADRESS):
		{
			TWBR = 0b00010111; //bit rate 23 => clk = 80.0 kHz
			set_twi_reciever_enable();
			//TWSR = (0<<TWPS0) | (0<<TWPS1); //Prescaler 0 0 -> 1
			TWAR = (1<<TWA5); // Address 010 0000, General Call Not Accepted
			break;
		}
		case(ST_ADRESS):
		{
			TWBR = 0b00010111; //bit rate 23 => clk = 80.0 kHz
			set_twi_reciever_enable();
			//TWSR = (0<<TWPS0) | (0<<TWPS1); //Prescaler 0 0 -> 1
			TWAR = (1<<TWA4) | (1<<TWGCE); // Address 001 0000, General Call Accepted
			break;
		}
	}
}

void set_twi_reciever_enable()
{
	TWCR = (1<<TWEA) | (1<<TWEN) | (1<<TWIE); //TWI enable, TWI interrupt enable
}

void Error()
{
	if(CONTROL != ARBITRATION)
	{
		stop_bus();
	}
	else
	{
		clear_int();
	}
}

void clear_int()
{
	TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWIE);
}

void start_bus()
{
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
}

void stop_bus()
{
	TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEA) | (1<<TWEN) | (1<<TWIE);
}

void set_data(int b)
{
	TWDR = b;
}

int get_data()
{
	return TWDR;
}

void send_bus_and_wait()
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}


bool send_status(int adr)
{
	start_bus();
	
	if(CONTROL != START)
	{
		Error();
		return false;
	}
	set_data(adr);
	send_bus_and_wait();
	
	if(CONTROL != ADRESS_W)
	{
		Error();
		return false;
	}
	set_data(I_STATUS);
	send_bus_and_wait();
	
	if(CONTROL == ARBITRATION)
	{
		return true;
	}
	else if(CONTROL != DATA_W)
	{
		Error();
		return false;
	}
	stop_bus();
	return true;
}

bool send_settings(int set)
{
	start_bus();
	
	if(CONTROL != START)
	{
		Error();
		return false;
	}
	set_data(C_ADRESS);
	send_bus_and_wait();
	
	if(CONTROL != ADRESS_W)
	{
		Error();
		return false;
	}
	set_data(I_SETTINGS);
	send_bus_and_wait();
	
	if(CONTROL != DATA_W)
	{
		Error();
		return false;
	}
	set_data(set);
	send_bus_and_wait();
	
	if(CONTROL != DATA_W)
	{
		Error();
		return false;
	}
	stop_bus();
	return true;
}

bool send_sweep(int pos)
{
	start_bus();
	if(CONTROL != START)
	{
		Error();
		return false;
	}
	set_data(S_ADRESS);
	send_bus_and_wait();
	if(CONTROL != ADRESS_W)
	{
		Error();
		return false;
	}
	set_data(I_SWEEP);
	send_bus_and_wait();
	if(CONTROL != DATA_W)
	{
		Error();
		return false;
	}
	set_data(pos);
	send_bus_and_wait();
	if(CONTROL != DATA_W)
	{
		Error();
		return false;
	}
	stop_bus();
	return true;
}

bool send_command(int direction, int rotation, int speed)
{
	start_bus();
	if(CONTROL != START)
	{
		Error();
		return false;
	}
	set_data(ST_ADRESS);
	send_bus_and_wait();
	if(CONTROL != ADRESS_W)
	{
		Error();
		return false;
	}
	set_data(I_COMMAND);
	send_bus_and_wait();
	if(CONTROL != DATA_W)
	{
		Error();
		return false;
	}
	set_data(direction);
	send_bus_and_wait();
	
	if(CONTROL != DATA_W)
	{
		Error();
		return false;
	}
	set_data(rot_elev);
	send_bus_and_wait();
	
	if(CONTROL != DATA_W)
	{
		Error();
		return false;
	}
	set_data(speed);
	send_bus_and_wait();
	
	if(CONTROL != DATA_W)
	{
		Error();
		return false;
	}
	stop_bus();
	return true;
}

bool send_elevation(int elevation)
{
	start_bus();
	
	if(CONTROL != START)
	{
		Error();
		return false;
	}
	set_data(ST_ADRESS);
	send_bus_and_wait();
	
	if(CONTROL != ADRESS_W)
	{
		Error();
		return false;
	}
	set_data(I_ELEVATION);
	send_bus_and_wait();
	
	if(CONTROL != DATA_W)
	{
		Error();
		return false;
	}
	set_data(elevation);
	send_bus_and_wait();
	
	if(CONTROL != DATA_W)
	{
		Error();
		return false;
	}
	stop_bus();
	return true;
}

bool send_sensors(int sens[7], int serv)
{
	start_bus();
	
	if(CONTROL != START)
	{
		Error();
		return false;
	}
	set_data(G_ADRESS); //General Call, NO instruction byte, NO NACK control of data
	send_bus_and_wait();
	
	if(CONTROL != ADRESS_W)
	{
		Error();
		return false;
	}
	for(int i=0; i < 7; ++i) //7 Sensors?
	{
		set_data(sens[i]);
		send_bus_and_wait();
		
	}
	set_data(serv);
	send_bus_and_wait();
	
	stop_bus();
	return true;
}

bool send_string(int adr, char str[])
{
	start_bus();
	
	if(CONTROL != START)
	{
		Error();
		return false;
	}
	set_data(adr);
	send_bus_and_wait();
	
	if(CONTROL != ADRESS_W)
	{
		Error();
		return false;
	}
	set_data(I_STRING);
	send_bus_and_wait();
	
	if(CONTROL != DATA_W)
	{
		Error();
		return false;
	}
	for(int i = 0; i < strlen(str); ++i)
	{
		set_data(str[i]);
		send_bus_and_wait();
		
	}
	stop_bus();
	return true;
}

bool send_string_fixed_length(int adr, uint8_t str[], int length)
{
	start_bus();
	
	if(CONTROL != START)
	{
		Error();
		return false;
	}
	set_data(adr);
	send_bus_and_wait();
	
	if(CONTROL != ADRESS_W)
	{
		Error();
		return false;
	}
	set_data(I_STRING);
	send_bus_and_wait();
	
	if(CONTROL != DATA_W)
	{
		Error();
		return false;
	}
	for(int i = 0; i < length; ++i)
	{
		set_data(str[i]);
		send_bus_and_wait();
		
	}
	stop_bus();
	return true;
}

bool send_something(int adr, int instruction, int packet)
{
	start_bus();
	
	if(CONTROL != START)
	{
		Error();
		return false;
	}
	set_data(adr);
	send_bus_and_wait();
	
	if(CONTROL != ADRESS_W)
	{
		Error();
		return false;
	}
	set_data(instruction);
	send_bus_and_wait();
	
	if(CONTROL != DATA_W)
	{
		Error();
		return false;
	}
	set_data(packet);
	send_bus_and_wait();
	
	if(CONTROL != DATA_W)
	{
		Error();
		return false;
	}
	stop_bus();
	return true;
}



void get_settings_from_bus()
{
	settings = get_data();
}

int get_settings()
{
	return settings;
}

void get_char_from_bus()
{
	message[message_counter] = get_data();
	message_counter += 1;
	message_length = message_counter;
}

void get_command_from_bus()
{
	command[current_command] = get_data();
	current_command += 1;
}

int get_command(int i)
{
	return command[i];
}

int get_message_length()
{
	return message_length;
}

char get_char(int i)
{
	return message[i];
}

void get_sensor_from_bus()
{
	if(sensor == 7)
	{
		for(int i = 0; i < sensor;++i)
		{
			sensors[i] = buffer[i];
		}
		servo = get_data();
	}
	else
	{
		buffer[sensor] = get_data();
		sensor += 1;
	}
}

int get_sensor(int i)
{
	return sensors[i];
}

int get_servo()
{
	return servo;
}

void get_sweep_from_bus()
{
	sweep = get_data();
}

int get_sweep()
{
	return sweep;
}

void stop_twi()
{
	current_command = 0;
	sensor = 0;
	message_counter = 0;
}

void reset_TWI()
{
	TWCR |= (1<<TWINT) | (1<<TWEA);
}


//TWI Interrupt vector MUHAHAHAHA
// ----------------------------------------------------------------------------- Communications
ISR(TWI_vect)
{
	switch(my_adress)
	{
		case(C_ADRESS):
		{
			if(CONTROL == SLAW || CONTROL == ARBIT_SLAW)
			{
				instruction = true;
				
			}
			else if(CONTROL == DATA_SLAW)
			{
				if(instruction)
				{
					current_instruction = get_data();
					instruction = false;
				}
				else
				{
					switch(current_instruction)
					{
						case(I_SETTINGS):
						{
							get_settings_from_bus();
							break;
						}
						case(I_STRING):
						{
							get_char_from_bus();
							break;
						}
					}
				}
			}
			else if (CONTROL == DATA_GENERAL)
			{
				get_sensor_from_bus();
			}
			else if (CONTROL == STOP)
			{
				stop_twi();
			}
			reset_TWI();
			break;
		}
		// ----------------------------------------------------------------------------- Sensors
		case(S_ADRESS):
		{
			if(CONTROL == SLAW || CONTROL == ARBIT_SLAW)
			{
				instruction = true;
			}
			else if(CONTROL == DATA_SLAW)
			{
				if(instruction)
				{
					current_instruction = get_data();
					instruction = false;
				}
				else
				{
					switch(current_instruction)
					{
						case(I_SWEEP):
						{
							get_sweep_from_bus();
							break;
						}
						case(I_STRING):
						{
							get_char_from_bus();
							break;
						}
					}
				}
			}
			else if (CONTROL == STOP)
			{
				stop_twi();
				switch(current_instruction)
				{
					case(I_STRING):
					{
						get_char_from_bus();
						break;
					}
				}
			}
			reset_TWI();
			break;
		}
		// ----------------------------------------------------------------------------- Steer
		case(ST_ADRESS):
		{
			if(CONTROL == SLAW || CONTROL == ARBIT_SLAW)
			{
				instruction = true;
			}
			else if(CONTROL == DATA_SLAW)
			{
				if(instruction)
				{
					current_instruction = get_data();
					instruction = false;
				}
				else
				{
					switch(current_instruction)
					{
						case(I_COMMAND):
						{
							get_command_from_bus();
							break;
						}
						case(I_STRING):
						{
							get_char_from_bus();
							break;
						}
					}
				}
			}
			else if (CONTROL == DATA_GENERAL)
			{
				get_sensor_from_bus();
			}
			else if (CONTROL == STOP)
			{
				stop_twi();
			}
			reset_TWI();
			break;
		}
	}
}