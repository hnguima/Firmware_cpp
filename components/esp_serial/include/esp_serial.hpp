#pragma once

#include <string>

#include <esp_err.h>
#include <driver/uart.h>

class Serial
{

public:
  Serial(int port);
  Serial(int port, uart_mode_t mode);
  Serial(int port, int baud_rate);
  Serial(uart_config_t config);
  Serial(uart_config_t config, uart_mode_t mode);

private:
  int port;
  int baud_rate;
  uart_word_length_t data_size;
  uart_parity_t parity;
  uart_stop_bits_t stop_bits;
  uart_hw_flowcontrol_t flow_control;
  uart_mode_t mode;
};
