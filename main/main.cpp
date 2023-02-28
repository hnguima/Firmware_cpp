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
#include "http_server.hpp"
#include "stepper_driver.hpp"
#include "esp_filesystem.hpp"

#include "esp_ethernet.hpp"

const auto sleep_time = std::chrono::milliseconds{10000};

#include <driver/gpio.h>

#define BTN_USER 36
#define AC_DET 39
#define ADC_BAT 34
#define ADC_EN 35
#define BARRAMENTO_ATI 32
#define ENABLE_3V 0

// GPIOS INPUT
#define GPIO_INPUT_PIN_SEL ((1ULL << BTN_USER) | (1ULL << AC_DET))

// GPIOS OUTPUT
#define GPIO_OUTPUT_PIN_SEL ((1ULL << BARRAMENTO_ATI) | (1ULL << ENABLE_3V))

extern "C" void app_main()
{

  // Entradas digitais
  gpio_config_t io_conf;
  io_conf.intr_type = GPIO_INTR_DISABLE;
  io_conf.mode = GPIO_MODE_INPUT;
  io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
  io_conf.pull_down_en = (gpio_pulldown_t)0;
  io_conf.pull_up_en = (gpio_pullup_t)0;
  gpio_config(&io_conf);

  // Saidas digitais
  io_conf.intr_type = GPIO_INTR_DISABLE;
  io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
  io_conf.mode = GPIO_MODE_OUTPUT;
  gpio_config(&io_conf);

  gpio_set_level((gpio_num_t)ENABLE_3V, 0);

  Ethernet *eth_driver = Ethernet::get_instance();
  // eth_driver->init_STA("centaurus", "d3sn3tw1f1");

  // WifiDriver *wifi_driver = WifiDriver::get_instance();
  // wifi_driver->init_STA("centaurus", "d3sn3tw1f1");

  HTTPServer *http_server = HTTPServer::get_instance();

  FileSystem *file_system = FileSystem::get_instance();
  file_system->mount_all();

  char file_data[11] = {};

  int size_read = file_system->read("/data/config", file_data, 10);
  ESP_LOGI("file", "%s \nsize: %d\n ", file_data, size_read);

  size_read = file_system->read("/data/config", file_data, 10);
  ESP_LOGI("file", "%s \nsize: %d\n ", file_data, size_read);

  size_read = file_system->read("/data/config", file_data, 10);
  ESP_LOGI("file", "%s \nsize: %d\n ", file_data, size_read);

  size_read = file_system->read("/data/config", file_data, 10);
  ESP_LOGI("file", "%s \nsize: %d\n ", file_data, size_read);
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