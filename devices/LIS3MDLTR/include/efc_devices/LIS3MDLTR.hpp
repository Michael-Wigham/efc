/**
 * @file EFC_LIS3MDLTR_HPP.hpp
 * @author Michael Wigham (michael@wigham.net)
 * @brief // TODO:
 * @date 2024-03-11
 *
 * @copyright Copyright (c) 2024
 */

#ifndef EFC_LIS3MDLTR_HPP
#define EFC_LIS3MDLTR_HPP

#include <efc_drivers/i2c_device.hpp>

#define LIS3MDLTR_DEFAULT_ADRESS 0x1C

#define LIS3MDLTR_WHO_AM_I 0x0F

#define LIS3MDLTR_CTRL_REG1 0x20
#define LIS3MDLTR_CTRL_REG2 0x21
#define LIS3MDLTR_CTRL_REG3 0x22
#define LIS3MDLTR_CTRL_REG4 0x23
#define LIS3MDLTR_CTRL_REG5 0x24

#define LIS3MDLTR_STATUS_REG 0x27

#define LIS3MDLTR_OUT_X_L 0x28
#define LIS3MDLTR_OUT_X_H 0x29
#define LIS3MDLTR_OUT_Y_L 0x2A
#define LIS3MDLTR_OUT_Y_H 0x2B
#define LIS3MDLTR_OUT_Z_L 0x2C
#define LIS3MDLTR_OUT_Z_H 0x2D

#define LIS3MDLTR_TEMP_OUT_L 0x2E
#define LIS3MDLTR_TEMP_OUT_H 0x2F

#define LIS3MDLTR_INT_CFG 0x30
#define LIS3MDLTR_INT_SRC 0x31

#define LIS3MDLTR_INT_THS_L 0x32
#define LIS3MDLTR_INT_THS_H 0x33

typedef enum {
  LIS3MDLTR_RANGE_4_GAUSS = 0b00,  ///< +/- 4g (default value)
  LIS3MDLTR_RANGE_8_GAUSS = 0b01,  ///< +/- 8g
  LIS3MDLTR_RANGE_12_GAUSS = 0b10, ///< +/- 12g
  LIS3MDLTR_RANGE_16_GAUSS = 0b11, ///< +/- 16g
} lis3mdltr_range_t;

/** The magnetometer data rate, includes FAST_ODR bit */
typedef enum {
  LIS3MDLTR_DATARATE_0_625_HZ = 0b0000, ///<  0.625 Hz
  LIS3MDLTR_DATARATE_1_25_HZ = 0b0010,  ///<  1.25 Hz
  LIS3MDLTR_DATARATE_2_5_HZ = 0b0100,   ///<  2.5 Hz
  LIS3MDLTR_DATARATE_5_HZ = 0b0110,     ///<  5 Hz
  LIS3MDLTR_DATARATE_10_HZ = 0b1000,    ///<  10 Hz
  LIS3MDLTR_DATARATE_20_HZ = 0b1010,    ///<  20 Hz
  LIS3MDLTR_DATARATE_40_HZ = 0b1100,    ///<  40 Hz
  LIS3MDLTR_DATARATE_80_HZ = 0b1110,    ///<  80 Hz
  LIS3MDLTR_DATARATE_155_HZ = 0b0001,   ///<  155 Hz (FAST_ODR + UHP)
  LIS3MDLTR_DATARATE_300_HZ = 0b0011,   ///<  300 Hz (FAST_ODR + HP)
  LIS3MDLTR_DATARATE_560_HZ = 0b0101,   ///<  560 Hz (FAST_ODR + MP)
  LIS3MDLTR_DATARATE_1000_HZ = 0b0111,  ///<  1000 Hz (FAST_ODR + LP)
} lis3mdltr_dataRate_t;

/** The magnetometer performance mode */
typedef enum {
  LIS3MDLTR_LOWPOWERMODE = 0b00,  ///< Low power mode
  LIS3MDLTR_MEDIUMMODE = 0b01,    ///< Medium performance mode
  LIS3MDLTR_HIGHMODE = 0b10,      ///< High performance mode
  LIS3MDLTR_ULTRAHIGHMODE = 0b11, ///< Ultra-high performance mode
} lis3mdltr_performancemode_t;

/** The magnetometer operation mode */
typedef enum {
  LIS3MDLTR_CONTINUOUSMODE = 0b00, ///< Continuous conversion
  LIS3MDLTR_SINGLEMODE = 0b01,     ///< Single-shot conversion
  LIS3MDLTR_POWERDOWNMODE = 0b11,  ///< Powered-down mode
} lis3mdltr_operationmode_t;


class LIS3MDLTR : I2CDevice {
public:
    LIS3MDLTR(uint8_t address = LIS3MDLTR_DEFAULT_ADRESS);

    int16_t read_temperature();

    std::array<int16_t, 3> mag_data();
};

#endif // ifndef EFC_LIS3MDLTR_HPP
