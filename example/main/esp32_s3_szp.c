#include <stdio.h>
#include "esp32_s3_szp.h"

static const char *TAG = "esp32_s3_szp";

/**********************************************************/
/*************************  I2C ↓ *************************/
i2c_master_bus_handle_t i2c_bus_handle;

esp_err_t bsp_i2c_init(void)
{
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = BSP_I2C_NUM,
        .scl_io_num = BSP_I2C_SCL,
        .sda_io_num = BSP_I2C_SDA,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &i2c_bus_handle));

    return ESP_OK;
}
/***************************  I2C ↑  **********************/
/**********************************************************/

/**********************************************************/
/****************    LCD触摸屏 ↓   *************************/
static SemaphoreHandle_t touch_mux = NULL;
static esp_lcd_touch_handle_t tp = NULL;
static esp_lcd_panel_io_handle_t ioHandle = NULL;

static void touch_callback()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(touch_mux, &xHigherPriorityTaskWoken);

    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

// 触摸屏初始化
esp_err_t bsp_touch_init(void)
{
    touch_mux = xSemaphoreCreateBinary();
    assert(touch_mux); 

    
    esp_lcd_panel_io_i2c_config_t io_config = ESP_LCD_TOUCH_IO_I2C_CST816S_CONFIG();
    if (esp_lcd_new_panel_io_i2c((i2c_master_bus_handle_t)i2c_bus_handle, &io_config, &ioHandle) != ESP_OK) {
        ESP_LOGE(TAG, "Touch IO I2C creation failed");
        return ESP_FAIL;
    }

    esp_lcd_touch_config_t tp_cfg = {
        .x_max = TOUCH_H_RES,
        .y_max = TOUCH_V_RES,
        .rst_gpio_num = GPIO_NUM_NC,
        .int_gpio_num = TOUCH_GPIO_INT,
        .levels = {
            .reset = 0,
            .interrupt = 1,
        },
        .flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = 0,
        },
        .interrupt_callback = touch_callback,
    };

    if (esp_lcd_touch_new_i2c_cst816s(ioHandle, &tp_cfg, &tp) != ESP_OK) {
        ESP_LOGE(TAG, "CST816s init failed");
        return ESP_FAIL;
    }

    return ESP_OK;
}

static void read_touch() {
    ESP_LOGI(TAG, "Reading touch data...");
    
    uint16_t touch_x[1];
    uint16_t touch_y[1];
    uint16_t touch_strength[1];
    uint8_t touch_cnt = 0;

    bool touchpad_pressed = esp_lcd_touch_get_coordinates(tp, touch_x, touch_y, touch_strength, &touch_cnt, 1);
    if (touchpad_pressed) {
        ESP_LOGI(TAG, "Touch detected: x=%d, y=%d, strength=%d", touch_x[0], touch_y[0], touch_strength[0]);
        // 这里可以添加处理触摸事件的代码
    } else {
        ESP_LOGI(TAG, "No touch detected");
    }
}

void touch_task(void *pvParameters) {
    while (1) {
        // 等待中断信号量
        if (xSemaphoreTake(touch_mux, 0) == pdTRUE) {
            esp_lcd_touch_read_data(tp);
            read_touch();
        }
        vTaskDelay(pdMS_TO_TICKS(20)); // 防止高频轮询
    }
}
/***************    LCD触摸屏 ↑   *************************/
/***********************************************************/