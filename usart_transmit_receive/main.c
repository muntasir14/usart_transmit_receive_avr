/*
 * usart_transmit_receive.c
 *
 * Created: 9/12/2024 6:38:12 AM
 * Author : muntasir
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define RCV 

void receive_init();
void show_result_init();

void transmit_init();
void send_byte(uint8_t byte);

#ifdef RCV
const uint8_t seven_segment_digits[10] = {
	0xC0,  // 0 => 11000000 (a, b, c, d, e, f ON)
	0xF9,  // 1 => 11111001 (b, c ON)
	0xA4,  // 2 => 10100100 (a, b, d, e, g ON)
	0xB0,  // 3 => 10110000 (a, b, c, d, g ON)
	0x99,  // 4 => 10011001 (b, c, f, g ON)
	0x92,  // 5 => 10010010 (a, c, d, f, g ON)
	0x82,  // 6 => 10000010 (a, c, d, e, f, g ON)
	0xF8,  // 7 => 11111000 (a, b, c ON)
	0x80,  // 8 => 10000000 (a, b, c, d, e, f, g ON)
	0x90   // 9 => 10010000 (a, b, c, d, f, g ON)
};
#endif

int main(void)
{
	#ifdef RCV
		receive_init();
	#else
		uint8_t count = 0;
		transmit_init();
	#endif
    while (1) 
    {
		#ifndef RCV
			if(!(PINB & (1 << 0))) {
				while(!(UCSRA & (1 << UDRE)));
				send_byte(++count);
				_delay_ms(300);
			}
		#endif
    }
}

#ifdef RCV
	void receive_init() {
	
		UCSRB |= (1 << RXEN) | (1 << RXCIE);
		UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
		UBRRL = 0x33;
		sei();
		show_result_init();
	}

	void show_result_init() {
		DDRC = 0xff;
		DDRD |= (1 << 6);
	}
	
	ISR(USART_RXC_vect) {
		uint8_t temp = UDR % 10;
		PORTC = seven_segment_digits[temp];
		if(seven_segment_digits[temp] & (1 << 6)) {
			PORTD |= (1 << 6);
		} else {
			PORTD &= ~(1 << 6);
		}
	}

#else

	void transmit_init() {
		UCSRB |= (1 << TXEN);
		UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
		UBRRL = 0x33;
		sei();
		DDRB &= ~(1 << 0);
		PORTB |= (1 << 0);
	}

	void send_byte(uint8_t byte) {
		UDR = byte + '0';
	}

#endif






