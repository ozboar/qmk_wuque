#pragma once

/* SPI Config for flash */
#define SPI_DRIVER SPIDQ
#define SPI_SCK_PIN B3
#define SPI_MOSI_PIN B5
#define SPI_MISO_PIN B4
#define SPI_MOSI_PAL_MODE 5
#define EXTERNAL_FLASH_SPI_SLAVE_SELECT_PIN C12

/* Wireless config */
#define WLS_KEYCODE_PAIR_TIME 2000
#define MD_BT_NAME "Zoom65 v3 ($)"
#define MD_BT1_NAME MD_BT_NAME
#define MD_BT2_NAME MD_BT_NAME
#define MD_BT3_NAME MD_BT_NAME
#define MD_BT4_NAME MD_BT_NAME
#define MD_BT5_NAME MD_BT_NAME

/* UART1 for wireless driver */
#define SERIAL_DRIVER SD1
#define SD1_TX_PIN A9
#define SD1_RX_PIN A10

/* UART3 for screen module */
#define SD3_TX_PIN C10
#define SD3_RX_PIN C11

/* Hardware configurations */
#define USB_POWER_EN_PIN A14
#define LED_POWER_EN_PIN B7
#define WIRELESS_SW_PIN C0
#define HS_BAT_CABLE_PIN B9
#define BAT_FULL_PIN B8

/* Misc configuration */
#define GRAVE_ESC_CTRL_OVERRIDE
