#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "spi.h"


#define SPI_PORT	PORTB
#define SPI_DDR		DDRB

#define MOSI	3
#define MISO	4
#define SCK		5

#define BUFFERSIZE	100

SPI_MODE mode;
uint8_t receiveBuffer[BUFFERSIZE];
volatile uint8_t bufferCursor = 0;


void spi_master_open(void);
void spi_slave_open(void);

uint8_t spi_transmit_byte(uint8_t data);
void spi_transmit_buffer(void *buffer, uint16_t n);
uint8_t spi_master_receive_byte(void);
uint8_t spi_slave_receive_byte(void);
void spi_master_receive_buffer(void *buffer, uint16_t n);
void spi_slave_receive_buffer(void *buffer, uint16_t n);
void spi_flush();


void spi_master_open(void) {
	mode = MASTER;
	SPI_DDR |= (1 << SCK) | (1 << MOSI);
	SPI_DDR &= ~(1 << MISO);
	SPI_PORT |= (1 << MISO); // pull-up resistor for MISO pin
	
	
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1); // mode; Prescale = 64
}


void spi_slave_open(void) {
	mode = SLAVE;
	SPI_DDR |= (1 << MISO);
	SPI_DDR &= ~(1 << MOSI);
	SPI_PORT |= (1 << MOSI);
	SPCR = (1 << SPE);
}


void spi_open(SPI_MODE mode) {
	if (mode == MASTER)
		spi_master_open();
	else
		spi_slave_open();
}


uint8_t spi_transmit_byte(uint8_t data) {
	SPDR = data;
	while (!(SPSR & (1 << SPIF))) {
		// wait for flag interrupt
	}
	return SPDR;
}


void spi_transmit_buffer(void *buffer, uint16_t len) {
	uint8_t *data = (uint8_t*)buffer;

	for (uint16_t i = 0; i < len; i++) {
		spi_transmit_byte(data[i]);
	}
}


void spi_write(uint8_t data) {
	spi_transmit_byte(data);
}


void spi_writeBuffer(void *buffer, uint16_t len) {
	spi_transmit_buffer(buffer, len);
}


uint8_t spi_master_receive_byte(void) {
	return spi_transmit_byte(0xFF);
}


uint8_t spi_slave_receive_byte(void) {
	while (!(SPSR & (1 << SPIF))) {
		// wait for flag interrupt
	}
	return SPDR;
}


uint8_t spi_read() {
	if (mode == MASTER)
		return spi_master_receive_byte();
	else
		return spi_slave_receive_byte();
}


void spi_master_receive_buffer(void *buffer, uint16_t n) {
	uint8_t *data = (uint8_t*)buffer;

	for (uint16_t i = 0; i < n; i++) {
		data[i] = spi_master_receive_byte();
	}
}


void spi_slave_receive_buffer(void *buffer, uint16_t n) {
	uint8_t *data = (uint8_t*)buffer;

	for (uint16_t i = 0; i < n; i++) {
		data[i] = spi_slave_receive_byte();
	}
}


void spi_readBuffer(void *buffer, uint16_t len) {
	if (mode == MASTER)
		return spi_master_receive_buffer(buffer, len);
	else
		return spi_slave_receive_buffer(buffer, len);
}


void spi_flush() {
	memset(receiveBuffer, 0, BUFFERSIZE);
	bufferCursor = 0;
}


ISR(SPI_STC_vect) {
	receiveBuffer[bufferCursor++] = SPDR;
	if (bufferCursor == BUFFERSIZE)
		bufferCursor = 0;
}