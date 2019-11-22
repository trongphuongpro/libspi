#ifndef LIB_SPI_H
#define LIB_SPI_H

#ifdef __cplusplus
extern "C" {
#endif


void spi_master_initialize(void);
void spi_slave_initialize(void);

uint8_t spi_master_transmit_byte(uint8_t);
void spi_master_transmit_buffer(uint8_t *, uint8_t);
uint8_t spi_master_receive_byte(void);
void spi_master_receive_buffer(uint8_t *, uint8_t);

uint8_t spi_slave_receive_byte(void);
void spi_slave_receive_buffer(uint8_t *, uint8_t);

#ifdef __cplusplus
}
#endif

#endif