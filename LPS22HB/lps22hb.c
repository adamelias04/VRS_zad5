#include "../LPS22HB/lps22hb.h"

#include <math.h>

static lps22hb_read_t lps22hb_read_cb = 0;
static lps22hb_write_t lps22hb_write_cb = 0;

static float reference_pressure = 1013.25f;

uint8_t lps22hb_read_reg(uint8_t reg, uint8_t *data, uint8_t len)
{
        if(lps22hb_read_cb == 0) return 1;
        return lps22hb_read_cb(LPS22HB_I2C_ADDR, reg, data, len);
}

uint8_t lps22hb_write_reg(uint8_t reg, uint8_t *data, uint8_t len)
{
        if(lps22hb_write_cb == 0) return 1;
        return lps22hb_write_cb(LPS22HB_I2C_ADDR, reg, data, len);
}

uint8_t lps22hb_init(lps22hb_read_t read_cb, lps22hb_write_t write_cb)
{
        uint8_t value;

        if(read_cb == 0 || write_cb == 0) return 1;
        lps22hb_read_cb = read_cb;
        lps22hb_write_cb = write_cb;

        if(lps22hb_read_reg(LPS22HB_WHO_AM_I, &value, 1)) return 1;
        if(value != LPS22HB_WHO_AM_I_VALUE) return 1;

        value = LPS22HB_CTRL_REG1_ODR_10HZ | LPS22HB_CTRL_REG1_EN_LPFP | LPS22HB_CTRL_REG1_BDU;
        return lps22hb_write_reg(LPS22HB_CTRL_REG1, &value, 1);
}

float lps22hb_get_pressure(void)
{
        uint8_t data[3];

        if(lps22hb_read_reg(LPS22HB_PRESS_OUT_XL, data, 3)) return 0.0f;

        int32_t raw = ((int32_t)data[2] << 16) | ((int32_t)data[1] << 8) | data[0];
        if(raw & 0x00800000) raw |= 0xFF000000;

        return raw / 4096.0f;
}

float lps22hb_get_temperature(void)
{
        uint8_t data[2];

        if(lps22hb_read_reg(LPS22HB_TEMP_OUT_L, data, 2)) return 0.0f;

        return (int16_t)(data[1] << 8 | data[0]) / 100.0f;
}

void lps22hb_set_reference_pressure(float pressure)
{
        reference_pressure = pressure;
}


float lps22hb_calc_altitude(float pressure)
{
        return 44330.0f * (1.0f - powf(pressure / reference_pressure, 0.1903f));
}
