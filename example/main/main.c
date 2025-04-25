#include <stdio.h>
#include "esp32_s3_szp.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "main";

void app_main(void)
{
    
    ESP_LOGI(TAG, "I2C初始化...");
    bsp_i2c_init();
    ESP_LOGI(TAG, "触摸屏初始化...");
    bsp_touch_init();
    ESP_LOGI(TAG, "监听触摸事件...");
    xTaskCreate(
        touch_task,
        "touch_task", 
        4096,
        NULL,
        configMAX_PRIORITIES - 2,
        NULL
    );
}







