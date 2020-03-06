/** 
 * @file spi_tiva.c
 * @brief Wrapper for SPI communication protocol of Tiva C.
 * @author Nguyen Trong Phuong (aka trongphuongpro)
 * @date 2020 Mar 2
 */


#include "spi.h"
#include <stdbool.h>
#include "driverlib/ssi.h"


static uint32_t SSIBase;
static SPI_DEVICEMODE mode;


void tiva_spi_open(uint32_t base, SPI_DEVICEMODE __mode) {
	SSIBase = base;
	mode = __mode;

	SSIEnable(SSIBase);
}


uint8_t spi_receive() {
	uint32_t data;

	// Master need to initiate the transmission
	if (mode == MASTER) {
		SSIDataPut(SSIBase, 0x00);
	}

	SSIDataGet(SSIBase, &data);

	return data;
}


void spi_receiveBuffer(void *buffer, uint16_t len) {
	uint8_t *data = (uint8_t*)buffer;

	for (uint16_t i = 0; i < len; i++) {
		data[i] = spi_receive();
	}
}


void spi_send(uint8_t data) {
	SSIDataPut(SSIBase, data);
}


void spi_sendBuffer(const void *buffer, uint16_t len) {
	const uint8_t *data = (const uint8_t*)buffer;

	for (uint16_t i = 0; i < len; i++) {
		SSIDataPut(SSIBase, data[i]);
	}
}

/* End file */