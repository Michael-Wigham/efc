/**
 * @file uart_controller.hpp
 * @author Szymon Wlodarczyk (szymonwlod03@gmail.com)
 * @brief //TODO
 * @date 2023-09-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <esp_log.h>

#include <efc_drivers/uart_controller.hpp>

UARTController::UARTController(int rx_pin, int tx_pin, char terminator, int baud_rate)
    : m_uart_id(m_global_uart_id++), m_terminator(terminator) {
    if (m_uart_id > UART_NUM_MAX - 1) {
        ESP_LOGE("UARTController", "Maximum number of UART ports (%d) exceeded (%d)", m_uart_id + 1, UART_NUM_MAX);
        return;
    }

    const uart_config_t uart_config = {
        .baud_rate = baud_rate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122, // Source: trust me bro
        .source_clk = UART_SCLK_DEFAULT,
    };

    uart_driver_install(m_uart_id, m_rx_buffer_size, m_tx_buffer_size, m_event_queue_size, &m_uart_event_queue, 0);
    uart_param_config(m_uart_id, &uart_config);
    uart_set_pin(m_uart_id, tx_pin, rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_enable_pattern_det_baud_intr(m_uart_id, m_terminator, 1, 1, 0, 0);
    uart_pattern_queue_reset(m_uart_id, m_event_queue_size);

    // Activate uart if initialisation succeeds
    m_is_active = true;
}

std::optional<std::string> UARTController::read_packet() {
    if (!m_is_active) {
        ESP_LOGE("UARTController", "Attempt to read from inactive UART port (%d)", m_uart_id);
        return {};
    }

    uart_event_t event;
    int pattern_offset;

    while (pdTRUE == xQueueReceive(m_uart_event_queue, static_cast<void *>(&event), 0)) {
        switch (event.type) {
        case UART_PATTERN_DET:
            pattern_offset = uart_pattern_pop_pos(m_uart_id);

            // Ignore empty erroneous (queue full) detections
            if (pattern_offset < 0)
                break;

            uart_read_bytes(m_uart_id, m_internal_buffer, pattern_offset + 1, 0);
            m_internal_buffer[pattern_offset] = '\0';
            return std::string(m_internal_buffer);

        // Hardware FIFO and software ring-buffer drop all buffered data
        case UART_FIFO_OVF:
        case UART_BUFFER_FULL:
            uart_flush_input(m_uart_id);
            xQueueReset(m_uart_event_queue);
            break;

        // All other events are ignored
        default:
            break;
        }
    }

    return {};
}

void UARTController::write_packet(bool include_terminator) {
    if (!m_is_active) {
        ESP_LOGE("UARTController", "Attempt to write to inactive UART port (%d)", m_uart_id);
        return;
    }
}
