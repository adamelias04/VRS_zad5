#ifndef LPS22HB_H_
#define LPS22HB_H_

#include <stdint.h>

#define LPS22HB_I2C_ADDR                0x5D

#define LPS22HB_WHO_AM_I                0x0F
#define LPS22HB_WHO_AM_I_VALUE  0xB1

#define LPS22HB_CTRL_REG1               0x10
#define LPS22HB_CTRL_REG2               0x11
#define LPS22HB_STATUS                  0x27
#define LPS22HB_PRESS_OUT_XL    0x28
#define LPS22HB_TEMP_OUT_L              0x2B

#define LPS22HB_CTRL_REG1_ODR_1HZ       0x10
#define LPS22HB_CTRL_REG1_ODR_10HZ      0x20
#define LPS22HB_CTRL_REG1_ODR_25HZ      0x30
#define LPS22HB_CTRL_REG1_EN_LPFP       0x08
#define LPS22HB_CTRL_REG1_BDU           0x02

typedef uint8_t (*lps22hb_read_t)(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint8_t len);
typedef uint8_t (*lps22hb_write_t)(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint8_t len);

uint8_t lps22hb_init(lps22hb_read_t read_cb, lps22hb_write_t write_cb);
uint8_t lps22hb_read_reg(uint8_t reg, uint8_t *data, uint8_t len);
uint8_t lps22hb_write_reg(uint8_t reg, uint8_t *data, uint8_t len);
float lps22hb_get_pressure(void);
float lps22hb_get_temperature(void);
void lps22hb_set_reference_pressure(float pressure);
float lps22hb_calc_altitude(float pressure);

#endif /* LPS22HB_H_ */
