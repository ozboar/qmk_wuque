#pragma once

#define ENCODER_DEFAULT_POS 0x3

/* SPI Config for spi flash*/
#define SPI_DRIVER SPIDQ
#define SPI_SCK_PIN B3
#define SPI_MOSI_PIN B5
#define SPI_MISO_PIN B4
#define SPI_MOSI_PAL_MODE 5
#define EXTERNAL_FLASH_SPI_SLAVE_SELECT_PIN C12

/* UART1 for wireless driver */
#define SERIAL_DRIVER SD1
#define SD1_TX_PIN A9
#define SD1_RX_PIN A10

/* UART3 for screen module */
#define SD3_TX_PIN C10
#define SD3_RX_PIN C11

#define GRAVE_ESC_GUI_OVERRIDE
