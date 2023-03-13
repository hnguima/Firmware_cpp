#include "modbus_gateway.hpp"

#include "modbus_converter.hpp"

static const char *TAG = "ModbusGateway";

std::vector<ModbusGateway *> ModbusGateway::gateways;

ModbusGateway::ModbusGateway() : server(nullptr),
                                 tcp_port(0),
                                 tcp_timeout(portMAX_DELAY),
                                 tcp_config_ok(false),
                                 serial(nullptr),
                                 rtu_uart_port(0),
                                 rtu_baud_rate(SERIAL_DEFAULT_BAUD_RATE),
                                 rtu_parity(SERIAL_DEFAULT_PARITY),
                                 rtu_stop_bits(SERIAL_DEFAULT_STOP_BITS),
                                 rtu_data_bits(SERIAL_DEFAULT_DATA_SIZE),
                                 rtu_timeout(portMAX_DELAY),
                                 rtu_config_ok(false)
{

  // for (ModbusGateway gateway : ModbusGateway::gateways)
  // {
  //   if (gateway.rtu_uart_port == this->rtu_uart_port)
  //   {
  //   }

  //   if (gateway.tcp_port == this->tcp_port)
  //   {
  //   }
  // }
  sprintf(this->task_name, "mdb_gw(%d)", ModbusGateway::gateways.size());
  xTaskCreate(ModbusGateway::task, this->task_name, 4096, (void *)this, 5, NULL);
}

void ModbusGateway::config_tcp(uint16_t port)
{
  for (ModbusGateway *gateway : ModbusGateway::gateways)
  {
    if (gateway->tcp_port == this->tcp_port)
    {
      ESP_LOGE(TAG, "there is already a gateway open on tcp port %d", this->tcp_port);
      return;
    }
  }

  this->server = new SocketServer(port);

  this->tcp_config_ok = true;
}

void ModbusGateway::config_rtu(uint32_t uart_port)
{

  this->init_rtu(uart_port, SERIAL_DEFAULT_BAUD_RATE, SERIAL_DEFAULT_PARITY,
                 SERIAL_DEFAULT_STOP_BITS, SERIAL_DEFAULT_DATA_SIZE);
}
void ModbusGateway::config_rtu(uint32_t uart_port, uint32_t baud_rate)
{
  this->init_rtu(uart_port, baud_rate, SERIAL_DEFAULT_PARITY,
                 SERIAL_DEFAULT_STOP_BITS, SERIAL_DEFAULT_DATA_SIZE);
}

void ModbusGateway::config_rtu(uint32_t uart_port, uint32_t baud_rate,
                               uart_parity_t parity, uart_stop_bits_t stop_bits,
                               uart_word_length_t data_bits)
{
  this->init_rtu(uart_port, baud_rate, parity, stop_bits, data_bits);
}

void ModbusGateway::init_rtu(uint32_t uart_port, uint32_t baud_rate,
                             uart_parity_t parity, uart_stop_bits_t stop_bits,
                             uart_word_length_t data_bits)
{

  for (ModbusGateway *gateway : ModbusGateway::gateways)
  {
    if (gateway->rtu_uart_port == this->rtu_uart_port)
    {
      ESP_LOGE(TAG, "there is already a gateway open on uart port %lu", this->rtu_uart_port);
      return;
    }
  }

  this->serial = new Serial(uart_port, baud_rate, data_bits, parity, stop_bits,
                            SERIAL_DEFAULT_FLOW_CONTROL, UART_MODE_RS485_HALF_DUPLEX);

  this->rtu_uart_port = uart_port;
  this->rtu_baud_rate = baud_rate;
  this->rtu_parity = parity;
  this->rtu_stop_bits = stop_bits;
  this->rtu_data_bits = data_bits;

  this->rtu_config_ok = true;
}

esp_err_t ModbusGateway::set_rtu_pins(int rx_pin, int tx_pin)
{
  return this->serial->set_pins(rx_pin, tx_pin);
}
esp_err_t ModbusGateway::set_rtu_pins(int rx_pin, int tx_pin, int rts_pin, int cts_pin)
{
  return this->serial->set_pins(rx_pin, tx_pin, rts_pin, cts_pin);
}

void ModbusGateway::set_rtu_timeout(uint32_t timeout)
{
  this->rtu_timeout = timeout;
}
void ModbusGateway::set_tcp_timeout(uint32_t timeout)
{
  this->tcp_timeout = timeout;
}

void ModbusGateway::wait_for_config()
{
  while (this->rtu_config_ok == false && this->tcp_config_ok == false)
  {
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }

  ModbusGateway::gateways.push_back(this);
}

void ModbusGateway::task(void *param)
{
  ModbusGateway *modbus_gateway = (ModbusGateway *)param;

  modbus_gateway->wait_for_config();

  ModbusConverter *modbus_converter = new ModbusConverter();
  int *client = new int;

  uint8_t tcp_data[256] = {0};
  uint16_t *tcp_size = new uint16_t;

  modbus_gateway->serial->on_recv(
      [modbus_converter, modbus_gateway, client, tcp_data, tcp_size](uint8_t *data, uint32_t size)
      {
        // ESP_LOG_BUFFER_HEXDUMP("rtu recv", data, size, ESP_LOG_INFO);

        modbus_converter->set_rtu_packet(data, size);

        modbus_converter->get_tcp_packet((uint8_t *)tcp_data, tcp_size);

        // ESP_LOG_BUFFER_HEXDUMP("rtu sending", tcp_data, tcp_size, ESP_LOG_INFO);

        modbus_gateway->server->send_bytes(*client, (uint8_t *)tcp_data, *tcp_size);

        *client = 0;
      });

  uint8_t rtu_data[256] = {0};
  uint16_t *rtu_size = new uint16_t;

  modbus_gateway->server->on_client_recv(
      [modbus_converter, modbus_gateway, client, rtu_data, rtu_size](int socket_client, uint8_t *data, size_t size)
      {
        // ESP_LOG_BUFFER_HEXDUMP("tcp recv", data, size, ESP_LOG_INFO);

        modbus_converter->set_tcp_packet(data, size);

        modbus_converter->get_rtu_packet((uint8_t *)rtu_data, rtu_size);

        // ESP_LOG_BUFFER_HEXDUMP("tcp sending", rtu_data, rtu_size, ESP_LOG_INFO);

        modbus_gateway->serial->send_bytes((uint8_t *)rtu_data, *rtu_size);

        *client = socket_client;
      });

  while (true)
  {
    // do something
    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}