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

  // Ethernet *eth_driver = Ethernet::get_instance();

  WifiDriver *wifi_driver = WifiDriver::get_instance();
  wifi_driver->init_STA("CLARO_2GA8652A", "38A8652A");
  // wifi_driver->init_STA("centaurus", "d3sn3tw1f1");

  HTTPServer *http_server = HTTPServer::get_instance();

  Settings<MainConfig> config("/data/main.set");

  FirmwareUpdate fw_update;

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

  // while (true)
  // {
  //   ss.str("Counting...");
  //   std::this_thread::sleep_for(sleep_time);
  //   ESP_LOGI(pcTaskGetName(nullptr), "%s", ss.str().c_str());
  // }
}