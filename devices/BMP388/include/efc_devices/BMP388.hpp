/**
 * @file BMP388.hpp
 * @author Szymon Wlodarczyk (Szymon Wlodarczyk)
 * @author Michael Wigham (michael@wigham.net)
 * @brief // TODO:
 * @date 2024-03-18
 *
 * @copyright Copyright (c) 2024
 */

#ifndef EFC_BMP388_HPP
#define EFC_BMP388_HPP

#include <efc_drivers/i2c_device.hpp>
#include <rom/ets_sys.h>

#define BMP388_DEFAULT_ADDRESS CONFIG_EFC_DEVICES_BMP388_SLAVE_ADDRESS

#define BMP388_REG_CMD 0x7E           /**< command register */
#define BMP388_REG_NVM_PAR_T1_L 0x31  /**< NVM PAR T1 low register */
#define BMP388_REG_NVM_PAR_T1_H 0x32  /**< NVM PAR T1 high register */
#define BMP388_REG_NVM_PAR_T2_L 0x33  /**< NVM PAR T2 low register */
#define BMP388_REG_NVM_PAR_T2_H 0x34  /**< NVM PAR T2 high register */
#define BMP388_REG_NVM_PAR_T3 0x35    /**< NVM PAR T3 register */
#define BMP388_REG_NVM_PAR_P1_L 0x36  /**< NVM PAR P1 low register */
#define BMP388_REG_NVM_PAR_P1_H 0x37  /**< NVM PAR P1 high register */
#define BMP388_REG_NVM_PAR_P2_L 0x38  /**< NVM PAR P2 low register */
#define BMP388_REG_NVM_PAR_P2_H 0x39  /**< NVM PAR P2 high register */
#define BMP388_REG_NVM_PAR_P3 0x3A    /**< NVM PAR P3 register */
#define BMP388_REG_NVM_PAR_P4 0x3B    /**< NVM PAR P4 register */
#define BMP388_REG_NVM_PAR_P5_L 0x3C  /**< NVM PAR P5 low register */
#define BMP388_REG_NVM_PAR_P5_H 0x3D  /**< NVM PAR P5 high register */
#define BMP388_REG_NVM_PAR_P6_L 0x3E  /**< NVM PAR P6 low register */
#define BMP388_REG_NVM_PAR_P6_H 0x3F  /**< NVM PAR P6 high register */
#define BMP388_REG_NVM_PAR_P7 0x40    /**< NVM PAR P7 register */
#define BMP388_REG_NVM_PAR_P8 0x41    /**< NVM PAR P8 register */
#define BMP388_REG_NVM_PAR_P9_L 0x42  /**< NVM PAR P9 low register */
#define BMP388_REG_NVM_PAR_P9_H 0x43  /**< NVM PAR P9 high register */
#define BMP388_REG_NVM_PAR_P10 0x44   /**< NVM PAR P10 register */
#define BMP388_REG_NVM_PAR_P11 0x45   /**< NVM PAR P11 register */
#define BMP388_REG_CONFIG 0x1F        /**< configure register */
#define BMP388_REG_ODR 0x1D           /**< odr register */
#define BMP388_REG_OSR 0x1C           /**< osr register */
#define BMP388_REG_PWR_CTRL 0x1B      /**< power control register */
#define BMP388_REG_IF_CONF 0x1A       /**< if configure register */
#define BMP388_REG_INT_CTRL 0x19      /**< interrupt control register */
#define BMP388_REG_FIFO_CONFIG_2 0x18 /**< fifo configure 2 register */
#define BMP388_REG_FIFO_CONFIG_1 0x17 /**< fifo configure 1 register */
#define BMP388_REG_FIFO_WTM_1 0x16    /**< fifo watermark 1 register */
#define BMP388_REG_FIFO_WTM_0 0x15    /**< fifo watermark 2 register */
#define BMP388_REG_FIFO_DATA 0x14     /**< fifo data register */
#define BMP388_REG_FIFO_LENGTH_1 0x13 /**< fifo length 1 register */
#define BMP388_REG_FIFO_LENGTH_0 0x12 /**< fifo length 0 register */
#define BMP388_REG_INT_STATUS 0x11    /**< interrupt status register */
#define BMP388_REG_EVENT 0x10         /**< event register */
#define BMP388_REG_SENSORTIME_2 0x0E  /**< sensor time 2 register */
#define BMP388_REG_SENSORTIME_1 0x0D  /**< sensor time 1 register */
#define BMP388_REG_SENSORTIME_0 0x0C  /**< sensor time 0 register */
#define BMP388_REG_DATA_5 0x09        /**< data 5 register - TEMP_MSB_23_16 */
#define BMP388_REG_DATA_4 0x08        /**< data 4 register - TEMP_LSB_15_8 */
#define BMP388_REG_DATA_3 0x07        /**< data 3 register - TEMP_XLSB_7_0 */
#define BMP388_REG_DATA_2 0x06        /**< data 2 register - PRESS_MSB_23_16 */
#define BMP388_REG_DATA_1 0x05        /**< data 1 register - PRESS_LSB_15_8 */
#define BMP388_REG_DATA_0 0x04        /**< data 0 register - PRESS_XLSB_7_0 */
#define BMP388_REG_STATUS 0x03        /**< status register */
#define BMP388_REG_ERR_REG 0x02       /**< error register */
#define BMP388_REG_CHIP_ID 0x00       /**< chip id register */

typedef enum {
    BMP388_OVERSAMPLING_x1 = 0x00,
    BMP388_OVERSAMPLING_x2 = 0x01,
    BMP388_OVERSAMPLING_x4 = 0x02,
    BMP388_OVERSAMPLING_x8 = 0x03,
    BMP388_OVERSAMPLING_x16 = 0x04,
    BMP388_OVERSAMPLING_x32 = 0x05
} bmp388_oversampling_t;

typedef enum {
    BMP388_MODE_SLEEP_MODE = 0b00000000,
    BMP388_MODE_FORCED_MODE = 0b00010000,
    BMP388_MODE_NORMAL_MODE = 0b00110000
} bmp388_mode_t;

typedef enum {
    BMP388_ODR_200_HZ = 0x00,
    BMP388_ODR_100_HZ = 0x01,
    BMP388_ODR_50_HZ = 0x02,
    BMP388_ODR_25_HZ = 0x03,
    BMP388_ODR_12P5_HZ = 0x04,
    BMP388_ODR_6P25_HZ = 0x05,
    BMP388_ODR_3P1_HZ = 0x06,
    BMP388_ODR_1P5_HZ = 0x07,
    BMP388_ODR_0P78_HZ = 0x08,
    BMP388_ODR_0P39_HZ = 0x09,
    BMP388_ODR_0P2_HZ = 0x0A,
    BMP388_ODR_0P1_HZ = 0x0B,
    BMP388_ODR_0P05_HZ = 0x0C,
    BMP388_ODR_0P02_HZ = 0x0D,
    BMP388_ODR_0P01_HZ = 0x0E,
    BMP388_ODR_0P006_HZ = 0x0F,
    BMP388_ODR_0P003_HZ = 0x10,
    BMP388_ODR_0P0015_HZ = 0x11
} bmp388_odr_t;

class BMP388 : I2CDevice {
public:
    BMP388(uint8_t address = BMP388_DEFAULT_ADDRESS);

    bmp388_odr_t get_odr();

    void set_odr(bmp388_odr_t odr);

    bmp388_oversampling_t get_pressure_oversampling();

    void set_pressure_oversampling(bmp388_oversampling_t oversampling);

    bmp388_oversampling_t get_temperature_oversampling();

    void set_temperature_oversampling(bmp388_oversampling_t oversampling);

    float get_temperature();

    float get_pressure();

    std::array<float, 2>  get_data();

private:
    uint16_t m_t1;
    uint16_t m_t2;
    int8_t m_t3;
    int16_t m_p1;
    int16_t m_p2;
    int8_t m_p3;
    int8_t m_p4;
    uint16_t m_p5;
    uint16_t m_p6;
    int8_t m_p7;
    int8_t m_p8;
    int16_t m_p9;
    int8_t m_p10;
    int8_t m_p11;
    int64_t m_t_fine;

    int64_t _comp_temperature(uint32_t data);

    int64_t _comp_pressure(uint32_t data);
};

#endif // ifndef EFC_BMP388_HPP
