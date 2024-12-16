#pragma once

#define ENCODER_DEFAULT_POS 0x3

/* SPI Config for spi flash*/
#define SPI_DRIVER SPIDQ
#define SPI_SCK_PIN B3
#define SPI_MOSI_PIN B5
#define SPI_MISO_PIN B4
#define SPI_MOSI_PAL_MODE 5
#define EXTERNAL_FLASH_SPI_SLAVE_SELECT_PIN C12

#define MD_BT1_NAME "Zoom65v3 (JB) BT$"
#define MD_BT2_NAME "Zoom65v3 (JB) BT$"
#define MD_BT3_NAME "Zoom65v3 (JB) BT$"
#define MD_BT4_NAME "Zoom65v3 (JB) BT$"
#define MD_BT5_NAME "Zoom65v3 (JB) BT$"
#define WLS_KEYCODE_PAIR_TIME 2000

/* UART1 for wireless driver */
#define SERIAL_DRIVER SD1
#define SD1_TX_PIN A9
#define SD1_RX_PIN A10

/* UART3 for screen module */
#define SD3_TX_PIN C10
#define SD3_RX_PIN C11

/* Peripheral config */
#define USB_POWER_EN_PIN A14
#define LED_POWER_EN_PIN B7
#define HS_BAT_CABLE_PIN B9
#define BAT_FULL_PIN B8
#define BAT_FULL_STATE 1

#define GRAVE_ESC_GUI_OVERRIDE
