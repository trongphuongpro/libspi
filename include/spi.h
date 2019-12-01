#ifndef __SPI__
#define __SPI__

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {MASTER=0, SLAVE=1} SPI_MODE;

void spi_open(SPI_MODE);

uint8_t spi_read();
void spi_readBuffer(void *buffer, uint16_t);

void spi_write(uint8_t);
void spi_writeBuffer(void *buffer, uint16_t);


#ifdef __cplusplus
}
#endif

#endif /* __SPI__ */