/** 
 * @file spi.h
 * @brief Function prototypes for SPI communication protocol
 * @author Nguyen Trong Phuong (aka trongphuongpro)
 * @date 2020 Jan 22
 */

#ifndef __SPI__
#define __SPI__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Device mode: Master or Slave
 */
typedef enum {MASTER=0, SLAVE} SPI_DEVICEMODE;


/**
 * @brief Data mode: mode 0, mode 1, mode 2 and mode 3.
 */
typedef enum {MODE0=0, MODE1, MODE2, MODE3} SPI_DATAMODE;


/**
 * @brief Trasmittion bit order: MSB or LSB
 */
typedef enum {MSB=0, LSB} SPI_BITORDER;


/**
 * @brief Initialize SPI bus.
 *
 * Default:
 * Master: data mode 0, MSB First.
 * Slave: data mode 0, MSB First, transfer complete interrupt.
 *
 * @param mode MASTER or SLAVE.
 * @return nothing.
 */
void spi_open(SPI_DEVICEMODE mode);


/**
 * @brief Set bit order for SPI transmitting.
 * @param order MSB or LSB.
 * @return nothing.
 */
void spi_setBitOrder(SPI_BITORDER order);


/**
 * @brief Set data mode for SPI transmitting.
 * @param mode MODE0, MODE1, MODE2 or MODE3.
 * @return nothing.
 */
void spi_setDataMode(SPI_DATAMODE mode);


/**
 * @brief Set clock rate divider.
 * 
 * Transmitting speed = MCU's speed / factor.
 * 
 * @param factor 2,4,8,16,32,64,128.
 * @return nothing.
 */
void spi_setClockDivider(uint8_t factor);


/**
 * @brief Read 1 byte from SPI bus.
 * @return one byte.
 */
uint8_t spi_read(void);


/**
 * @brief Read 1 array from SPI bus.
 * @param buffer pointer to array.
 * @param len the length of data array.
 * @return nothing.
 */
void spi_readBuffer(void *buffer, uint16_t len);


/**
 * @brief Write 1 byte to SPI bus.
 * @return nothing.
 */
void spi_write(uint8_t data);


/**
 * @brief Write 1 array to SPI bus.
 * @param buffer pointer to array.
 * @param len the length of data array.
 * @return nothing.
 */
void spi_writeBuffer(void *buffer, uint16_t len);


#ifdef __cplusplus
}
#endif

#endif /* __SPI__ */