#include "esp_serial.hpp"

#include <freertos/FreeRTOS.h>

#include <esp_log.h>
#include <esp_event.h>

static const char *TAG = "Serial";

#define SERIAL_DEFAULT_BAUD_RATE 115200
#define SERIAL_DEFAULT_DATA_SIZE UART_DATA_8_BITS
#define SERIAL_DEFAULT_PARITY UART_PARITY_DISABLE
#define SERIAL_DEFAULT_STOP_BITS UART_STOP_BITS_1
#define SERIAL_DEFAULT_FLOW_CONTROL UART_HW_FLOWCTRL_DISABLE
#define SERIAL_DEFAULT_MODE UART_MODE_UART

Serial::Serial(int port) : port(port),
                           baud_rate(SERIAL_DEFAULT_BAUD_RATE),
                           data_size(SERIAL_DEFAULT_DATA_SIZE),
                           parity(SERIAL_DEFAULT_PARITY),
                           stop_bits(SERIAL_DEFAULT_STOP_BITS),
                           flow_control(SERIAL_DEFAULT_FLOW_CONTROL),
                           mode(UART_MODE_UART)
{

  /* Configure parameters of an UART driver,
   * communication pins and install the driver */
  uart_config_t uart_config = {
      .baud_rate = this->baud_rate,
      .data_bits = this->data_bits,
      .parity = this->parity,
      .stop_bits = this->stop_bits,
      .flow_ctrl = this->flow_ctrl,
      .source_clk = UART_SCLK_DEFAULT,
  };
  // Install UART driver, and get the queue.
  uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart0_queue, 0);
  uart_param_config(EX_UART_NUM, &uart_config);

  // Set UART log level
  esp_log_level_set(TAG, ESP_LOG_INFO);
  // Set UART pins (using UART0 default pins ie no changes.)
  uart_set_pin(EX_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

  // Set uart pattern detect function.
  uart_enable_pattern_det_baud_intr(EX_UART_NUM, '+', PATTERN_CHR_NUM, 9, 0, 0);
  // Reset the pattern queue length to record at most 20 pattern positions.
  uart_pattern_queue_reset(EX_UART_NUM, 20);

  // Create a task to handler UART event from ISR
  xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);
}

Serial::Serial(int port) : port(port),
                           baud_rate(SERIAL_DEFAULT_BAUD_RATE),
                           data_size(SERIAL_DEFAULT_DATA_SIZE),
                           parity(SERIAL_DEFAULT_PARITY),
                           stop_bits(SERIAL_DEFAULT_STOP_BITS),
                           flow_control(SERIAL_DEFAULT_FLOW_CONTROL),
                           mode(UART_MODE_UART)
{

  /* Configure parameters of an UART driver,
   * communication pins and install the driver */
  uart_config_t uart_config = {
      .baud_rate = this->baud_rate,
      .data_bits = this->data_bits,
      .parity = this->parity,
      .stop_bits = this->stop_bits,
      .flow_ctrl = this->flow_ctrl,
      .source_clk = UART_SCLK_DEFAULT,
  };
  // Install UART driver, and get the queue.
  uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart0_queue, 0);
  uart_param_config(EX_UART_NUM, &uart_config);

  // Set UART log level
  esp_log_level_set(TAG, ESP_LOG_INFO);
  // Set UART pins (using UART0 default pins ie no changes.)
  uart_set_pin(EX_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

  // Set uart pattern detect function.
  uart_enable_pattern_det_baud_intr(EX_UART_NUM, '+', PATTERN_CHR_NUM, 9, 0, 0);
  // Reset the pattern queue length to record at most 20 pattern positions.
  uart_pattern_queue_reset(EX_UART_NUM, 20);

  // Create a task to handler UART event from ISR
  xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);
}