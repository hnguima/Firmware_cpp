#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <string>
#include <sstream>
#include <esp_pthread.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>

// #include "esp_wifi_driver.hpp"
#include "stepper_driver.hpp"
#include "esp_filesystem.hpp"

#include "esp_wifi_driver.hpp"
#include "esp_ethernet.hpp"
#include "firmware_update.hpp"
#include "http_server.hpp"

#include "esp_settings.hpp"

#include "socket_client_tcp.hpp"
#include "socket_server_tcp.hpp"

#include "main.pb.h"

#include <driver/gpio.h>

#define ENABLE_3V 0
#define GPIO_OUTPUT_PIN_SEL (1ULL << ENABLE_3V)

const auto sleep_time = std::chrono::milliseconds{10000};

extern "C" void app_main()
{
  // Saidas digitais
  gpio_config_t io_conf;
  io_conf.intr_type = GPIO_INTR_DISABLE;
  io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
  io_conf.mode = GPIO_MODE_OUTPUT;
  gpio_config(&io_conf);

  gpio_set_level((gpio_num_t)ENABLE_3V, 0);

  FileSystem *file_system = FileSystem::get_instance();
  file_system->mount_all();

  SocketServer *socket_server = new SocketServer(502);

  vTaskDelay(2000 / portTICK_PERIOD_MS);

  Ethernet *eth_driver = Ethernet::get_instance();

  // WifiDriver *wifi_driver = WifiDriver::get_instance();
  // wifi_driver->init_STA("CLARO_2GA8652A", "38A8652A");
  // wifi_driver->init_STA("centaurus", "d3sn3tw1f1");

  HTTPServer *http_server = HTTPServer::get_instance();

  // Settings<MainConfig> config("/data/main.set");

  FirmwareUpdate fw_update;

  SocketClient *sock = new SocketClient("192.168.2.200", 8080);
  // SocketClient *sock2 = new SocketClient();
  // SocketClient *sock3 = new SocketClient();

  // Stepper *new_stepper1 = new Stepper();
  // Stepper *new_stepper2 = new Stepper();
  // Stepper *new_stepper3 = new Stepper();
  // Stepper *new_stepper4 = new Stepper();

  std::stringstream ss;

  // Wifi obj;

  // obj.myNum = 15;
  // // obj.myString = "Some text";

  // // Print attribute values
  // std::cout << obj.myNum << "\n";
  // std::cout << myObj.myString;

  // ss << "Hello World";
  // ESP_LOGI(pcTaskGetName(nullptr), "%s", ss.str().c_str());
  int *i = (int *)malloc(sizeof(int));
  *i = 0;

  sock->on_recv([](uint8_t *buffer, size_t size)
                { ESP_LOG_BUFFER_CHAR("recv client", buffer, size); });

  socket_server->on_client_recv([socket_server](int client, uint8_t *buffer, size_t size)
                                { 
                                  struct sockaddr_in address;
                                  int address_size = sizeof(address);
                                  getpeername(client, (struct sockaddr *)&address, (socklen_t *)&address_size);

                                  ESP_LOGI("recv server", "recv from %s:%d ", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
                                  ESP_LOG_BUFFER_CHAR("recv server", buffer, size); });

  // while (true)
  // {

  //   ESP_LOGI("main", "local i: %d", *i);
  //   (*i)++;

  //   std::this_thread::sleep_for(sleep_time);
  // }
}