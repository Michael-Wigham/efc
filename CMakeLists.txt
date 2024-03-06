set(EFC_SRCS)
set(EFC_INCLUDE_DIRS)

list(APPEND EFC_SRCS
    "efc_common/common.cpp"
)
list(APPEND EFC_INCLUDE_DIRS
    "efc_common/include"
)

if(CONFIG_EFC_DRIVER_ADC_ENABLED)
    list(APPEND EFC_SRCS
        "efc_drivers/adc/adc.cpp"
    )
    list(APPEND EFC_INCLUDE_DIRS
        "efc_drivers/adc/include"
    )
endif()

if(CONFIG_EFC_DRIVER_FLASH_ENABLED)
    list(APPEND EFC_SRCS
        "efc_drivers/flash/flash_controller.cpp"
    )
    list(APPEND EFC_INCLUDE_DIRS
        "efc_drivers/flash/include"
    )
endif()

if(CONFIG_EFC_DRIVER_GPIO_ENABLED)
    list(APPEND EFC_SRCS
        "efc_drivers/gpio/gpio.cpp"
    )
    list(APPEND EFC_INCLUDE_DIRS
        "efc_drivers/gpio/include"
    )
endif()

if(CONFIG_EFC_DRIVER_I2C_ENABLED)
    list(APPEND EFC_SRCS
        "efc_drivers/i2c/i2c_controller.cpp"
    )
    list(APPEND EFC_INCLUDE_DIRS
        "efc_drivers/i2c/include"
    )
endif()

if(CONFIG_EFC_DRIVER_PWM_ENABLED)
    list(APPEND EFC_SRCS
        "efc_drivers/pwm/pwm_controller.cpp"
    )
    list(APPEND EFC_INCLUDE_DIRS
        "efc_drivers/pwm/include"
    )
endif()

if(CONFIG_EFC_DRIVER_UART_ENABLED)
    list(APPEND EFC_SRCS
        "efc_drivers/uart/uart_controller.cpp"
    )
    list(APPEND EFC_INCLUDE_DIRS
        "efc_drivers/uart/include"
    )
endif()

if(CONFIG_EFC_DEVICES_HCSR04_ENABLED)
    list(APPEND EFC_SRCS
        "devices/HCSR04/HCSR04.cpp"
    )
    list(APPEND EFC_INCLUDE_DIRS
        "devices/HCSR04/include"
    )
endif()

if(CONFIG_EFC_DEVICES_LSM6DSOWTR_ENABLED)
    list(APPEND EFC_SRCS
        "devices/lsm6dsowtr/lsm6dsowtr.cpp"
    )
    list(APPEND EFC_INCLUDE_DIRS
        "devices/lsm6dsowtr/include"
    )
endif()

if(CONFIG_EFC_DEVICES_TCS3472X_ENABLED)
    list(APPEND EFC_SRCS
        "devices/TCS3472X/TCS3472X.cpp"
    )
    list(APPEND EFC_INCLUDE_DIRS
        "devices/TCS3472X/include"
    )
endif()

idf_component_register(
    SRCS ${EFC_SRCS}
    INCLUDE_DIRS ${EFC_INCLUDE_DIRS}
    REQUIRES driver esp_adc
    PRIV_REQUIRES driver esp_adc esp_partition esp_timer
)

target_compile_options(${COMPONENT_LIB} PUBLIC -std=c++23)
