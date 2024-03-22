/**
 * @file TCS3472X.hpp
 * @author Michael Wigham (michael@wigham.net)
 * @brief //TODO
 * @date 2024-03-05
 *
 * @copyright Copyright (c) 2024
 */

// EXAMPLE https://github.com/libdriver/tcs34725/blob/master/src/driver_tcs34725.c

#ifndef TCS3472X_HPP
#define TCS3472X_HPP

#include <efc_drivers/i2c_device.hpp>

// TCS3472X I2C Address
#define TCS3472X_ADDRESS 0x29

#define TCS3472X_CONTROL_BIT 0x80

// TCS3472X Enable register address
#define TCS3472X_ENABLE (0x00 | TCS3472X_CONTROL_BIT)  // Enable Register
#define TCS3472X_TIMING (0x01 | TCS3472X_CONTROL_BIT)  // RGBC Timing Register
#define TCS3472X_WAIT (0x03 | TCS3472X_CONTROL_BIT)    // Wait Time Register
#define TCS3472X_AILTL (0x04 | TCS3472X_CONTROL_BIT)   // RGBC clear channel low threshold lower byte Register
#define TCS3472X_AILTH (0x05 | TCS3472X_CONTROL_BIT)   // RGBC clear channel low threshold upper byte Register
#define TCS3472X_AIHTL (0x06 | TCS3472X_CONTROL_BIT)   // RGBC clear channel high threshold lower byte Register
#define TCS3472X_AIHTH (0x07 | TCS3472X_CONTROL_BIT)   // RGBC clear channel high threshold upper byte Register
#define TCS3472X_PERS (0x0C | TCS3472X_CONTROL_BIT)    // Persistence Register
#define TCS3472X_CONFIG (0x0D | TCS3472X_CONTROL_BIT)  // Configuration Register
#define TCS3472X_CONTROL (0x0F | TCS3472X_CONTROL_BIT) // Control Register
#define TCS3472X_ID (0x12 | TCS3472X_CONTROL_BIT)      // ID Register
#define TCS3472X_STATUS (0x13 | TCS3472X_CONTROL_BIT)  // Status Register
#define TCS3472X_CDATAL (0x14 | TCS3472X_CONTROL_BIT)  // Clear channel data low byte Register
#define TCS3472X_CDATAH (0x15 | TCS3472X_CONTROL_BIT)  // Clear channel data high byte Register
#define TCS3472X_RDATAL (0x16 | TCS3472X_CONTROL_BIT)  // Red channel data low byte Register
#define TCS3472X_RDATAH (0x17 | TCS3472X_CONTROL_BIT)  // Red channel data high byte Register
#define TCS3472X_GDATAL (0x18 | TCS3472X_CONTROL_BIT)  // Green channel data low byte Register
#define TCS3472X_GDATAH (0x19 | TCS3472X_CONTROL_BIT)  // Green channel data high byte Register
#define TCS3472X_BDATAL (0x1A | TCS3472X_CONTROL_BIT)  // Blue channel data low byte Register
#define TCS3472X_BDATAH (0x1B | TCS3472X_CONTROL_BIT)  // Blue channel data high byte Register

// Enable register values
#define TCS3472X_ENABLE_WEN 0x08 // Wait Enable - activates the wait timer
#define TCS3472X_ENABLE_AEN 0x02 // RGBC Enable - actives the ADC, 0 disables it
#define TCS3472X_ENABLE_PON 0x01 // Power on - activates the internal oscillator, 0 disables it

// Timing register values
#define TCS3472X_INTEGRATIONTIME_2_4MS 0xFF // 2.4ms - 1 cycle - Max Count: 1024
#define TCS3472X_INTEGRATIONTIME_24MS 0xF6  // 24.0ms - 10 cycles - Max Count: 10240
#define TCS3472X_INTEGRATIONTIME_50MS 0xEB  // 50.4ms - 21 cycles - Max Count: 21504
#define TCS3472X_INTEGRATIONTIME_60MS 0xE7  // 60.0ms - 25 cycles - Max Count: 25700
#define TCS3472X_INTEGRATIONTIME_101MS 0xD6 // 100.8ms - 42 cycles - Max Count: 43008
#define TCS3472X_INTEGRATIONTIME_120MS 0xCE // 120.0ms - 50 cycles - Max Count: 51200
#define TCS3472X_INTEGRATIONTIME_154MS 0xC0 // 153.6ms - 64 cycles - Max Count: 65535
#define TCS3472X_INTEGRATIONTIME_180MS 0xB5 // 180.0ms - 75 cycles - Max Count: 65535
#define TCS3472X_INTEGRATIONTIME_199MS 0xAD // 199.2ms - 83 cycles - Max Count: 65535
#define TCS3472X_INTEGRATIONTIME_240MS 0x9C // 240.0ms - 100 cycles - Max Count: 65535
#define TCS3472X_INTEGRATIONTIME_300MS 0x83 // 300.0ms - 125 cycles - Max Count: 65535
#define TCS3472X_INTEGRATIONTIME_360MS 0x6A // 360.0ms - 150 cycles - Max Count: 65535
#define TCS3472X_INTEGRATIONTIME_401MS 0x59 // 400.8ms - 167 cycles - Max Count: 65535
#define TCS3472X_INTEGRATIONTIME_420MS 0x51 // 420.0ms - 175 cycles - Max Count: 65535
#define TCS3472X_INTEGRATIONTIME_480MS 0x38 // 480.0ms - 200 cycles - Max Count: 65535
#define TCS3472X_INTEGRATIONTIME_499MS 0x30 // 499.2ms - 208 cycles - Max Count: 65535
#define TCS3472X_INTEGRATIONTIME_540MS 0x1F // 540.0ms - 225 cycles - Max Count: 65535
#define TCS3472X_INTEGRATIONTIME_600MS 0x06 // 600.0ms - 250 cycles - Max Count: 65535
#define TCS3472X_INTEGRATIONTIME_614MS 0x00 // 614.4ms - 256 cycles - Max Count: 65535

// Wait Time register values
#define TCS3472X_WTIME_2_4MS 0xFF // WLONG0 = 2.4ms   WLONG1 = 0.029s
#define TCS3472X_WTIME_204MS 0xAB // WLONG0 = 204ms   WLONG1 = 2.45s
#define TCS3472X_WTIME_614MS 0x00 // WLONG0 = 614ms   WLONG1 = 7.4s

// Persistence Register values
#define TCS3472X_PERS_NONE 0b0000
#define TCS3472X_PERS_1_CYCLE 0b0001
#define TCS3472X_PERS_2_CYCLE 0b0010
#define TCS3472X_PERS_3_CYCLE 0b0011
#define TCS3472X_PERS_5_CYCLE 0b0100
#define TCS3472X_PERS_10_CYCLE 0b0101
#define TCS3472X_PERS_15_CYCLE 0b0110
#define TCS3472X_PERS_20_CYCLE 0b0111
#define TCS3472X_PERS_25_CYCLE 0b1000
#define TCS3472X_PERS_30_CYCLE 0b1001
#define TCS3472X_PERS_35_CYCLE 0b1010
#define TCS3472X_PERS_40_CYCLE 0b1011
#define TCS3472X_PERS_45_CYCLE 0b1100
#define TCS3472X_PERS_50_CYCLE 0b1101
#define TCS3472X_PERS_55_CYCLE 0b1110
#define TCS3472X_PERS_60_CYCLE 0b1111

typedef enum {
    TCS3472X_GAIN_1X = 0x00,  /**<  No gain  */
    TCS3472X_GAIN_4X = 0x01,  /**<  4x gain  */
    TCS3472X_GAIN_16X = 0x02, /**<  16x gain */
    TCS3472X_GAIN_60X = 0x03  /**<  60x gain */
} tcs3472X_gain_t;

class TCS3472X : I2CDevice {
public:
    TCS3472X(uint8_t address = TCS3472X_ADDRESS, uint8_t integration_time = TCS3472X_INTEGRATIONTIME_24MS, tcs3472X_gain_t gain = TCS3472X_GAIN_1X);

    void enable();

    void disable();

    void setIntegrationTime(uint8_t integration_time);

    void setGain(tcs3472X_gain_t gain);

    void getRawData(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c);

    void getRawDataOneShot(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c);

    void getRGB(float *r, float *g, float *b);

private:
    uint8_t m_integration_time;
    tcs3472X_gain_t m_gain;
};

#endif  // TCS3472X_HPP
