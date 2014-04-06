/*
 * kommunikationsmodulen.c
 *
 * Created: 4/3/2014 10:12:45 AM
 *  Author: samli177
 */ 


#define F_CPU 18432000
#define BaudRate 115200

#define POLY 0x8408

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>
#include "twi.h"

// -- Global variables --
uint8_t gTxPayload [255];
uint8_t gTxBuffer [517]; // if there are only control octets in data it needs to be 255 + 7 + 255 = 517
uint8_t gRxBuffer [517];
uint16_t gRxBufferIndex;

// -- Global variables from TWI --
int my_adress;
bool instruction;
int current_instruction;


// -- FIFO --

struct fifo {
	uint16_t size;           /* size of buffer in bytes */
	uint16_t read;           /* read pointer */
	uint16_t write;          /* write pointer */
	unsigned char buffer[]; /* fifo ring buffer */
};

// define a FIFO type for 'size' bytes 
#define MK_FIFO(size) \
struct fifo_ ## size {                  \
	struct fifo f;                      \
	unsigned char buffer_bytes[size];   \
}

// define a fifo (type: struct fifo *) with name 'name' for 's' bytes 
#define DEFINE_FIFO(name, s)                                \
struct fifo_##s _raw_##name = { .f = { .size = s } };   \
struct fifo *name = &_raw_##name.f;

uint8_t FifoDataLength (struct fifo *fifo)
{
	return (fifo->write - fifo->read) & (fifo->size -1);
};

uint8_t FifoWrite (struct fifo *fifo, unsigned char data)
{
	// fifo full : error
	if (FifoDataLength(fifo) == (fifo->size - 1))
	{
		return 1;
	}
	// write data & increment write pointer
	fifo->buffer[fifo->write] = data;
	fifo->write = (fifo->write + 1) & (fifo->size - 1);
	return 0;
};


uint8_t FifoRead (struct fifo *fifo, unsigned char *data)
{
	// fifo empty : error
	if (FifoDataLength(fifo) == 0)
	{
		return 1;
	}
	// read data & increment read pointer
	*data = fifo->buffer[fifo->read];
	fifo->read = (fifo->read + 1) & (fifo->size - 1);
	return 0;
};

// define FIFO for received packets (USART)
MK_FIFO(4096); // use 4 kB 
DEFINE_FIFO(gRxFIFO, 4096);

void init()
{
	DDRA |= (1<<PORTA0|1<<PORTA1); //set status diodes to outputs
	gRxBufferIndex = 0;
}

void initSerial()
{
	//set baud rate
	//typecasting of "int" to byte truncates to the lowest uint8_t
	UBRR0H = (uint8_t) (((F_CPU / 16 / BaudRate ) - 1)>>8);
	UBRR0L = (uint8_t) ((F_CPU / 16 / BaudRate ) - 1) ;

	//enable receiver and transmitter and enable receive interrupt
	UCSR0B = (1<<RXEN0)|(1<<TXEN0|(1<<RXCIE0));

	//Frame format: 8data, no parity, 1 stop bit 
	UCSR0C = (1<<UCSZ00 | 1<<UCSZ01);

}

uint8_t CheckRxComplete()
{
	return (UCSR0A & (1<<RXC0)); // zero if no data is available to read
}

uint8_t CheckTxReady()
{
	return (UCSR0A & (1<<UDRE0)); // zero if transmit register is not ready to receive new data
}

void WriteByte(uint8_t DataByteOut)
{
	while(CheckTxReady() == 0)
	{;;} // wait until ready to transmit NOTE: Can probably be optimized using interrupts
	UDR0 = DataByteOut;
}

uint8_t ReadByte()
{
	// NOTE: check if data is available before calling this function. Should probably be implemented w. interrupt.
	return UDR0;

}




// calculate crc for packet
uint16_t crc16(uint8_t tag, uint8_t length)
{
	int count;
	uint16_t data, i;
	unsigned int crc; // maybe use uint16_t here
	
	crc = 0xffff;
	
	if (length == 0)
	return (~crc);
	
	for(count = -2; count < length ; ++count)
	{
		if(count == -2)
		{
			data = tag;
		}else if(count == -1)
		{
			data = length;
		}else
		{
			data = gTxPayload[count];
		}
		
		for (i = 0; i < 8; ++i)
		{
			if ((crc & 0x0001) ^ (data & 0x0001))
			{
				crc = (crc >> 1) ^ POLY;
			}else
			{
				crc >>= 1;
			}
			
			data >>= 1;
		}
	}
	
	crc = ~crc;
	
	data = crc;
	crc = (crc << 8 | (data >> 8 & 0xff));
	
	return (crc);
}

void SendPacket(char tag, uint8_t length)
{
	int count, offset, buffersize;
	uint16_t crc;
	
	// put start of packet into transmit buffer
	gTxBuffer[0] = 0x7e; // frame delimiter
	gTxBuffer[1] = tag;
	gTxBuffer[2] = length;
	
	// payload
	
	offset = 3;
	
	for(count = 0; count < length; ++count)
	{		
		if(gTxPayload[count] == 0x7e || gTxPayload[count] == 0x7d) // if frame delimiter or escape octet occurs in data
		{
			gTxBuffer[count + offset] = 0x7d; // add the escape octet
			offset++;
			gTxBuffer[count + offset] = (1<<5)^gTxPayload[count]; // invert bit 5 in data
		}else
		{
			gTxBuffer[count + offset] = gTxPayload[count];
		}	
	}
	
	crc = crc16(tag, length);
	
	gTxBuffer[count+offset] = (uint8_t)(crc >> 8);
	++count;
	gTxBuffer[count + offset] = (uint8_t)(crc);
	++count;
	
	
	gTxBuffer[count+offset] = 0x7e;
	
	buffersize = count + offset;
	
	for(count = 0; count < buffersize + 1; ++count)
	{
		WriteByte(gTxBuffer[count]);
	}
}

void SerialSendMessage(char msg[])
{
	for(int i = 0; i < strlen(msg); ++i )
	{
		gTxPayload[i] = msg[i];
	}
	
	SendPacket('M', strlen(msg));
}
	

void SendPacket2(char tag, uint8_t length)
{
	WriteByte(0x7e);
	WriteByte(tag);
	WriteByte(length);
	for (int i = 0; i < length; ++i)
	{
		WriteByte(gTxPayload[i]);
	}
	WriteByte(0x00);
	WriteByte(0x00);
	WriteByte(0x7e);
	
}

uint8_t DecodeMessageRxFIFO()
{
	
	uint8_t *len = 0;
	uint8_t *character = 0;
	
	if(FifoRead(gRxFIFO, len))
	{
		send_string(S_ADRESS, "RxFIFO ERROR: LEN MISSING");
		return 1; // error
	}
	
	int length = *len; // I don't know why I can't use *len directly... but it took me 4h to figure out that you can't do it....
	
	//NOTE: there has to be a better way of doing this...
	int ifzero = 0;
	if(length == 0) ifzero = 1;
	uint8_t msg[length-1+ifzero];

	for(int i = 0; i < length; ++i)
	{
		if(FifoRead(gRxFIFO, character))
		{
			send_string(S_ADRESS, "RxFIFO ERROR: DATA MISSING");
			return 1; // error
		}

		msg[i] = *character;
	}
	
	
	// TODO: send to relevant party... the display for now
	send_string_fixed_length(S_ADRESS, msg, length);
	return 0;
}

void DecodeRxFIFO()
{
	uint8_t *tag = 0;
	
	if(!(FifoRead(gRxFIFO, tag))) // if the buffer is NOT empty
	{
		switch(*tag){
			case('M'): // if 'tag' is 'M'
			{
				if(DecodeMessageRxFIFO()) // if decoding failed
				{
					// TODO: flush buffer?
					return;
				}
				
				break;
			}
		}
	}
}

void bounce()
{
	for(int i = 0; i < gRxBuffer[1]; i++)
	{
		gTxPayload[i] = gRxBuffer[i+2];
	}
	SendPacket(gRxBuffer[0], gRxBuffer[1]);
}




// -- MAIN --





int main(void)
{
	init();
	initSerial();
	
	// init TWI
	my_adress = C_ADRESS;
	init_TWI(my_adress);
	
	sei();

	
    while(1)
    {
		PORTA ^= (1<<PORTA0);
		
		DecodeRxFIFO();
		
		_delay_ms(1000);
		
    }
}





// -- Interrupts -- 

ISR (USART0_RX_vect)
{
	uint8_t data;
	data = UDR0; // read data from buffer TODO: add check for overflow
	
	
	
	if(data == 0x7e)
	{
		if(gRxBufferIndex >= 4 || gRxBufferIndex == gRxBuffer[1] + 4) //TODO: add crc check
		{
			//temp
			 PORTA ^= (1<<PORTA1); // turn on/off led
			//temp		
			
			bounce();
			
			// Add packet (no crc) to fifo-buffer to cue it for decoding
			for(int i = 0; i < gRxBuffer[1] + 2; ++i)
			{
				FifoWrite(gRxFIFO, gRxBuffer[i]);
			}
		}
		
		gRxBufferIndex = 0; // always reset buffer index when frame delimiter (0x7e) is read 
		
	}else
	{
		gRxBuffer[gRxBufferIndex] = data;
		++gRxBufferIndex;
	}
		
	
}


// -- interrupt vector from TWI --
ISR(TWI_vect)
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
		switch(current_instruction)
		{
			case(I_SETTINGS):
			{
				get_settings();
				break;
			}
			case(I_STRING):
			{
				//get_char(1);
				break;
			}
		}
	}
	reset_TWI();
}