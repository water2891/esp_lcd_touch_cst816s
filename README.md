# esp_lcd_touch_cst816s

[中文](README_CN.md)

If espressif/esp_lcd_touch_cst816s doesn't work on your cst816s, try my modified version.

1. Comment these two lines in 'esp_lcd_touch_new_i2c_cst816s'

```c
    /* Reset controller */
    //ESP_GOTO_ON_ERROR(reset(cst816s), err, TAG, "Reset failed");
    /* Read product id */
    //ESP_GOTO_ON_ERROR(read_id(cst816s), err, TAG, "Read version failed");
    *tp = cst816s;
```

2. Change DATA_START_REG to 0x01

3. Add gesture in data_t struct

```c
    typedef struct {
        uint8_t gesture; //define gesture
        uint8_t num;
        uint8_t x_h : 4;
        uint8_t : 4;
        uint8_t x_l;
        uint8_t y_h : 4;
        uint8_t : 4;
        uint8_t y_l;
    } data_t;

    data_t point;
    ESP_RETURN_ON_ERROR(i2c_read_bytes(tp, DATA_START_REG, (uint8_t *)&point, sizeof(data_t)), TAG, "I2C read failed");

    portENTER_CRITICAL(&tp->data.lock);
    point.num = point.gesture > 0 ? 1 : point.num; //fix point.num by gesture data, because you'll get 0 when single click
    point.num = (point.num > POINT_NUM_MAX ? POINT_NUM_MAX : point.num);
```
