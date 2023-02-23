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

#include "esp_wifi_driver.hpp"
#include "http_server.hpp"
#include "stepper_driver.hpp"
#include "esp_filesystem.hpp"

const auto sleep_time = std::chrono::milliseconds{10000};

extern "C" void app_main()
{

  WifiDriver *wifi_driver = WifiDriver::get_instance();
  wifi_driver->init_STA("centaurus", "d3sn3tw1f1");

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