#pragma once

#include <inttypes.h>
#include <string.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_types.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_touch_cst816s/esp_lcd_touch_cst816s.h"

/******************************************************************************/
/***************************  I2C ↓ *******************************************/
#define BSP_I2C_SDA           (GPIO_NUM_47)   // SDA引脚
#define BSP_I2C_SCL           (GPIO_NUM_48)   // SCL引脚

#define BSP_I2C_NUM           (0)            // I2C外设
#define BSP_I2C_FREQ_HZ       100000         // 100kHz

esp_err_t bsp_i2c_init(void);   // 初始化I2C接口
/***************************  I2C ↑  *******************************************/
/*******************************************************************************/

/***********************************************************/
/****************    LCD触摸屏 ↓   *************************/
#define CST816S_ADDR        0x15

#define TOUCH_I2C_NUM       (0)

#define TOUCH_H_RES         (240)
#define TOUCH_V_RES         (240)

#define TOUCH_GPIO_INT      (GPIO_NUM_45)

#define TOUCH_I2C_FREQ      100000

esp_err_t bsp_touch_init(void);
void touch_task(void *pvParameters);
/***************    LCD触摸屏 ↑   *************************/
/***********************************************************/