#pragma once

#define HAL_USE_I2C TRUE
#define HAL_USE_SPI TRUE
#define SPI_USE_WAIT TRUE
#define SPI_SELECT_MODE SPI_SELECT_MODE_PAD
#define PAL_USE_CALLBACKS TRUE

#include_next <halconf.h>
