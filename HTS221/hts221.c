#include "hts221.h"

static hts221_read_t hts221_read_cb = 0;
static hts221_write_t hts221_write_cb = 0;

static float t0_degc, t1_degc;
static int16_t t0_out, t1_out;
static float h0_rh, h1_rh;
static int16_t h0_t0_out, h1_t0_out;

uint8_t hts221_read_reg(uint8_t reg, uint8_t *data, uint8_t len)
{
        if(hts221_read_cb == 0) return 1;
        if(len > 1) reg |= HTS221_AUTO_INCREMENT;
        return hts221_read_cb(HTS221_I2C_ADDR, reg, data, len);
}

uint8_t hts221_write_reg(uint8_t reg, uint8_t *data, uint8_t len)
{
        if(hts221_write_cb == 0) return 1;
        if(len > 1) reg |= HTS221_AUTO_INCREMENT;
        return hts221_write_cb(HTS221_I2C_ADDR, reg, data, len);
}

static uint8_t hts221_read_calibration(void)
{
        uint8_t c[HTS221_CALIB_LEN];

        if(hts221_read_reg(HTS221_CALIB_START, c, HTS221_CALIB_LEN)) return 1;

        h0_rh = c[0] / 2.0f;
        h1_rh = c[1] / 2.0f;

        t0_degc = (((uint16_t)(c[5] & 0x03) << 8) | c[2]) / 8.0f;
        t1_degc = (((uint16_t)(c[5] & 0x0C) << 6) | c[3]) / 8.0f;

        h0_t0_out = (int16_t)(c[7] << 8 | c[6]);
        h1_t0_out = (int16_t)(c[11] << 8 | c[10]);
        t0_out = (int16_t)(c[13] << 8 | c[12]);
        t1_out = (int16_t)(c[15] << 8 | c[14]);

        return 0;
}

uint8_t hts221_init(hts221_read_t read_cb, hts221_write_t write_cb)
{
        uint8_t value;

        if(read_cb == 0 || write_cb == 0) return 1;
        hts221_read_cb = read_cb;
        hts221_write_cb = write_cb;

        if(hts221_read_reg(HTS221_WHO_AM_I, &value, 1)) return 1;
        if(value != HTS221_WHO_AM_I_VALUE) return 1;

        value = HTS221_CTRL_REG1_PD | HTS221_CTRL_REG1_BDU | HTS221_CTRL_REG1_ODR_7HZ;
        if(hts221_write_reg(HTS221_CTRL_REG1, &value, 1)) return 1;

        return hts221_read_calibration();
}

float hts221_get_temperature(void)
{
        uint8_t data[2];

        if(hts221_read_reg(HTS221_TEMP_OUT_L, data, 2)) return 0.0f;
        int16_t t_out = (int16_t)(data[1] << 8 | data[0]);

        return t0_degc + (float)(t_out - t0_out) * (t1_degc - t0_degc) / (float)(t1_out - t0_out);
}

float hts221_get_humidity(void)
{
        uint8_t data[2];

        if(hts221_read_reg(HTS221_HUMIDITY_OUT_L, data, 2)) return 0.0f;
        int16_t h_out = (int16_t)(data[1] << 8 | data[0]);

        float humidity = h0_rh + (float)(h_out - h0_t0_out) * (h1_rh - h0_rh) / (float)(h1_t0_out - h0_t0_out);

        if(humidity < 0.0f) humidity = 0.0f;
        if(humidity > 100.0f) humidity = 100.0f;
        return humidity;
}
