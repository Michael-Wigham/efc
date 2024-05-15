/**
 * @file lsm6dsowtr.hpp
 * @author Michael Wigham (michael@wigham.net)
 * @brief // TODO:
 * @date 2024-01-24
 *
 * @copyright Copyright (c) 2024
 */

#ifndef EFC_LSM6DSOWTR_HPP
#define EFC_LSM6DSOWTR_HPP

#include <efc_drivers/i2c_device.hpp>

#define LSM6DSOWTR_DEFAULT_ADRESS CONFIG_EFC_DEVICES_LSM6DSOWTR_SLAVE_ADDRESS

#define FUNC_CFG_ACCESS_REG 0x01        // Enable embedded functions register

#define SENSOR_SYNC_TIME_FRAME_REG 0x04     // Sensor sync configuration register
#define SENSOR_SYNC_RES_RATIO_REG 0x05      // Sensor synchronization resolution ratio

#define FIFO_CTRL1_REG 0x06                 // FIFO control register
#define FIFO_CTRL2_REG 0x07                 // FIFO control register
#define FIFO_CTRL3_REG 0x08                 // FIFO control register
#define FIFO_CTRL4_REG 0x09                 // FIFO control register
#define FIFO_CTRL5_REG 0x0A                 // FIFO control register

#define DRDY_PULSE_CFG_G_REG 0x0B           // DataReady configuration register

#define INT1_CTRL_REG 0x0D                  // INT1 pad control register
#define INT2_CTRL_REG 0x0E                  // INT2 pad control register

#define WHO_AM_I_REG 0x0F                   // Who am i register

#define CTRL1_XL 0x10                       // Linear acceleration sensor control register 1

#define CTRL2_G 0x11                        // Angular rate sensor control register 2
#define CTRL3_C 0x12                        // Control register 3
#define CTRL4_C 0x13                        // Control register 4
#define CTRL5_C 0x14                        // Control register 5
#define CTRL6_C 0x15                        // Angular rate sensor control register 6
#define CTRL7_G 0x16                        // Angular rate sensor control register 7
#define CTRL8_XL 0x17                       // Linear acceleration sensor control register 8
#define CTRL9_XL 0x18                       // Linear acceleration sensor control register 9
#define CTRL10_C 0x19                       // Control register 10

#define MASTER_CONFIG 0x1A                  // Master configuration register

#define WAKE_UP_SRC 0x1B                    // Wake up interrupt source register
#define TAP_SRC 0x1C                        // Tap source register

#define D6D_SRC 0x1D                        // Portrait, landscape, face-up and face-down source register

#define STATUS_REG 0x1E                     // The STATUS_REG register is read by the SPI/I2C interface

#define OUT_TEMP_L 0x20                     // Temperature data output register (r). L and H registers together express a 16-bit word in two’s complement
#define OUT_TEMP_H 0x21                     // Temperature data output register (r). L and H registers together express a 16-bit word in two’s complement

#define OUTX_L_G 0x22                       // Angular rate sensor pitch axis (X) angular rate output register (r). The value is expressed as a 16-bit word in two’s complement.
#define OUTX_H_G 0x23                       // Angular rate sensor pitch axis (X) angular rate output register (r). The value is expressed as a 16-bit word in two’s complement.
#define OUTY_L_G 0x24                       // Angular rate sensor roll axis (Y) angular rate output register (r).The value is expressed as a 16-bit word in two’s complement.
#define OUTY_H_G 0x25                       // Angular rate sensor roll axis (Y) angular rate output register (r). The value is expressed as a 16-bit word in two’s complement.
#define OUTZ_L_G 0x26                       // Angular rate sensor roll axis (Z) angular rate output register (r). The value is expressed as a 16-bit word in two’s complement.
#define OUTZ_H_G 0x27                       // Angular rate sensor roll axis (Z) angular rate output register (r). The value is expressed as a 16-bit word in two’s complement.

#define OUTX_L_XL 0x28                      // Linear acceleration sensor X-axis output register (r). The value is expressed as a 16-bit word in two’s complement.
#define OUTX_H_XL 0x29                      // Linear acceleration sensor X-axis output register (r). The value is expressed as a 16-bit word in two’s complement.
#define OUTY_L_XL 0x2A                      // Linear acceleration sensor Y-axis output register (r). The value is expressed as a 16-bit word in two’s complement.
#define OUTY_H_XL 0x2B                      // Linear acceleration sensor Y-axis output register (r). The value is expressed as a 16-bit word in two’s complement.
#define OUTZ_L_XL 0x2C                      // Linear acceleration sensor Z-axis output register (r). The value is expressed as a 16-bit word in two’s complement.
#define OUTZ_H_XL 0x2D                      // Linear acceleration sensor Z-axis output register (r). The value is expressed as a 16-bit word in two’s complement.



class LSM6DSOWTR : I2CDevice {
public:
    LSM6DSOWTR(uint8_t address = LSM6DSOWTR_DEFAULT_ADRESS);

    /**
     * @brief Read the temperature data from the device
     *
     * @return int16_t
     */
    float read_temperature();

    /**
     * @brief return gx, gy, gz
     *
     * @return std::array<int16_t, 3>
     */
    std::array<float, 3> read_gyroscope();
    float read_gyroscope_x();
    float read_gyroscope_y();
    float read_gyroscope_z();

    /**
     * @brief Return x, y, z
     *
     * @return std::array<int16_t, 3>
     */
    std::array<float, 3> read_accelerometer();
    float read_accelerometer_x();
    float read_accelerometer_y();
    float read_accelerometer_z();

private:
    float gyroscope_sensitivity = 250.0f;
    float accelerometer_sensitivity = 9.81f * 2.0f;
};

#endif // ifndef EFC_LSM6DSOWTR_HPP
