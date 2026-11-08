#ifndef HTS221_H_
#define HTS221_H_

#include <stdint.h>

#define HTS221_I2C_ADDR                 0x5F

#define HTS221_WHO_AM_I                 0x0F
#define HTS221_WHO_AM_I_VALUE   0xBC

#define HTS221_AV_CONF                  0x10
#define HTS221_CTRL_REG1                0x20
#define HTS221_CTRL_REG2                0x21
#define HTS221_CTRL_REG3                0x22
#define HTS221_STATUS_REG               0x27
#define HTS221_HUMIDITY_OUT_L   0x28
#define HTS221_TEMP_OUT_L               0x2A
#define HTS221_CALIB_START              0x30
#define HTS221_CALIB_LEN                16

#define HTS221_CTRL_REG1_PD             0x80
#define HTS221_CTRL_REG1_BDU    0x04
#define HTS221_CTRL_REG1_ODR_1HZ        0x01
#define HTS221_CTRL_REG1_ODR_7HZ        0x02
#define HTS221_CTRL_REG1_ODR_12HZ       0x03

#define HTS221_AUTO_INCREMENT   0x80

typedef uint8_t (*hts221_read_t)(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint8_t len);
typedef uint8_t (*hts221_write_t)(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint8_t len);

uint8_t hts221_init(hts221_read_t read_cb, hts221_write_t write_cb);
uint8_t hts221_read_reg(uint8_t reg, uint8_t *data, uint8_t len);
uint8_t hts221_write_reg(uint8_t reg, uint8_t *data, uint8_t len);
float hts221_get_temperature(void);
float hts221_get_humidity(void);

#endif /* HTS221_H_ */
