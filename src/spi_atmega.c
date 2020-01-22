/** 
 * @file spi_atmega.c
 * @brief Implementation for SPI communication protocol
 * @author Nguyen Trong Phuong (aka trongphuongpro)
 * @date 2020 Jan 22
 */

#include <avr/io.h>
#include "spi.h"

#define SPI_PORT	PORTB
#define SPI_DDR		DDRB

#define MOSI	3
#define MISO	4
#define SCK		5
#define SS		2


static SPI_DEVICEMODE mode;

static void spi_master_open(void);
static void spi_slave_open(void);
static uint8_t spi_transmit_byte(uint8_t data);
static void spi_transmit_buffer(void *buffer, uint16_t n);
static uint8_t spi_master_receive_byte(void);
static uint8_t spi_slave_receive_byte(void);
static void spi_master_receive_buffer(void *buffer, uint16_t len);
static void spi_slave_receive_buffer(void *buffer, uint16_t len);


void spi_master_open(void) {
	mode = MASTER;
	SPI_DDR |= (1 << SCK) | (1 << MOSI);
	SPI_DDR &= ~(1 << MISO);
	SPI_PORT |= (1 << MISO); // pull-up resistor for MISO pin
	
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1); // data mode 0; Prescale = 64
}


void spi_slave_open(void) {
	mode = SLAVE;
	SPI_DDR |= (1 << MISO);
	SPI_DDR &= ~((1 << MOSI) | (1 << SS));
	SPI_PORT |= (1 << MOSI) | (1 << SS);

	SPCR = (1 << SPE) | (1 << SPIE); // enable interrupt
}


void spi_open(SPI_DEVICEMODE mode) {
	if (mode == MASTER)
		spi_master_open();
	else
		spi_slave_open();
}


void spi_setClockDivider(uint8_t factor) {
	if (mode == MASTER) {
		switch (factor) {
			case 4: case 16: case 64: case 128:	SPSR &= ~(1 << SPI2X);
												break;

			case 2: case 8: case 32:	SPSR &= ~(1 << SPI2X);
										SPSR |= (1 << SPI2X);
										break;								
		}

		switch (factor) {
			case 2: case 4:	SPCR &= ~(3 << SPR0);
							break;

			case 8: case 16:	SPCR &= ~(3 << SPR0);
								SPCR |= (1 << SPR0);
								break;

			case 32: case 64:	SPCR &= ~(3 << SPR0);
								SPCR |= (2 << SPR0);
								break;

			case 128:	SPCR &= ~(3 << SPR0);
						SPCR |= (3 << SPR0);
						break;
		}
	}
}


void spi_setBitOrder(SPI_BITORDER order) {
	SPCR &= ~(1 << DORD);
	SPCR |= (order << DORD);
}


void spi_setDataMode(SPI_DATAMODE mode) {
	SPCR &= ~(3 << CPHA);
	SPCR |= (mode << CPHA);
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


/*ISR(SPI_STC_vect) {
	receiveData = SPDR;
}*/