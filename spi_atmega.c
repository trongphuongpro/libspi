#include <avr/io.h>
#include "libspi.h"


#define SPI_PORT	PORTB
#define SPI_DDR		DDRB

#define MOSI	3
#define MISO	4
#define SCK		5


void spi_master_initialize(void) {
	SPI_DDR |= (1 << SCK) | (1 << MOSI);
	SPI_DDR &= ~(1 << MISO);
	SPI_PORT |= (1 << MISO); // pull-up resistor for MISO pin
	
	
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1); // mode; Prescale = 64
}


void spi_slave_initialize(void) {
	SPI_DDR |= (1 << MISO);
	SPI_DDR &= ~(1 << MOSI);
	SPI_PORT |= (1 << MOSI);
	SPCR = (1 << SPE) | (1 << SPR0) | (1 << SPR1);
}


void spi_master_transmit_buffer(uint8_t *buffer, uint8_t n) {
	for (uint8_t i = 0; i < n; i++) {
		spi_master_transmit_byte(buffer[i]);
	}
}


uint8_t spi_master_transmit_byte(uint8_t data) {

	SPDR = data;
	while (!(SPSR & (1 << SPIF))) {
		// wait for flag interrupt
	}

	return SPDR;
}


uint8_t spi_master_receive_byte(void) {
	return spi_master_transmit_byte(0xFF);
}


void spi_master_receive_buffer(uint8_t *buffer, uint8_t n) {
	for (uint8_t i = 0; i < n; i++) {
		buffer[i] = spi_master_receive_byte();
	}
}


void spi_slave_receive_buffer(uint8_t *buffer, uint8_t n) {
	for (uint8_t i = 0; i < n; i++) {
		buffer[i] = spi_slave_receive_byte();
	}
}


uint8_t spi_slave_receive_byte(void) {
	while (!(SPSR & (1 << SPIF))) {
		// wait for flag interrupt
	}
	return SPDR;
}
