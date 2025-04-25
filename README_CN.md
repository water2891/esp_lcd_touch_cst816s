# esp_lcd_touch_cst816s

如果你的cst816s用 espressif/esp_lcd_touch_cst816s 时无法正常工作，试试我下面的方法：

1. 在 'esp_lcd_touch_new_i2c_cst816s' 中注释掉两行代码

如果你的cst816s在初始化的时候报错，就把下面两行代码注释掉，因为在它主动触发中断前你没法进行I2C通信。

```c
    /* Reset controller */
    //ESP_GOTO_ON_ERROR(reset(cst816s), err, TAG, "Reset failed");
    /* Read product id */
    //ESP_GOTO_ON_ERROR(read_id(cst816s), err, TAG, "Read version failed");
    *tp = cst816s;
```

2. 把 DATA_START_REG 改成 0x01，以便下一步你可以读取到“手势”信息

3. Add gesture in data_t struct

```c
    typedef struct {
        uint8_t gesture; //加一个手势字段
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
    point.num = point.gesture > 0 ? 1 : point.num; //单击屏幕时，point.num会是0，所以要用手势数据来纠正下。
    point.num = (point.num > POINT_NUM_MAX ? POINT_NUM_MAX : point.num);
```
