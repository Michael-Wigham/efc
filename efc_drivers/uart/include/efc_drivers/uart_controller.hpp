/**
 * @file uart_controller.hpp
 * @author Szymon Wlodarczyk (szymonwlod03@gmail.com)
 * @brief //TODO
 * @date 2023-09-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef EFC_UART_CONTROLLER_HPP
#define EFC_UART_CONTROLLER_HPP

#include <driver/uart.h>

#include <optional>
#include <string>

class UARTController {
public:
    /**
     * @brief A uart reader that splits the incomming data stream into packets delimited by the terminator character.
     *
     * @param rx_pin Input pin (default for ESP32-WROOM is GPIO3)
     * @param tx_pin Output pin (default for ESP32-WROOM is GPIO1)
     * @param terminator Character to split packets on
     * @param baud_rate UART baud rate (defaults to 115200)
     */
    UARTController(int rx_pin, int tx_pin, char terminator, int baud_rate = 115200);

    /**
     * @brief Reads a packet from the uart stream, if available
     */
    std::optional<std::string> read_packet();

    /** //TODO
     * @brief Writes a packet to UART
     *
     * @param include_terminator If true will append the terminator to the end of the packet (defaults to true)
     */
    void write_packet(bool include_terminator = true);

private:
    // Static configuration
    inline static int m_global_uart_id = UART_NUM_0;
    inline static const int m_rx_buffer_size = 512;
    inline static const int m_tx_buffer_size = 0;
    inline static const int m_rx_timeout_ms = 0;
    inline static const int m_event_queue_size = 20;

    // Instance state
    uart_port_t m_uart_id;
    QueueHandle_t m_uart_event_queue;
    bool m_is_active = false;
    char m_internal_buffer[m_rx_buffer_size];
    char m_terminator;
};

#endif // ifndef EFC_UART_CONTROLLER_HPP
