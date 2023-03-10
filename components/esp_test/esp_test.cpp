#include "esp_serial.hpp"

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "unity.h"

static const char *TAG = "test";

TestManager::TestManager()
{
}

TestManager::TestManager(handler_func_t setup)
{
}

TestManager::TestManager(handler_func_t setup, handler_func_t finish)
{
}

void TestManager::set_setup_func(handler_func_t setup)
{
}

void TestManager::set_finish_func(handler_func_t finish)
{
}

void TestManager::start()
{

  if (this->setup && (this->setup() == false))
  {
    ESP_LOGI(TAG, "Setup returned false, not running tests");
    return;
  }

  this->run_tests();

  int err;

  if (err = this->finish && (this->finish() == false))
  {
    ESP_LOGE(TAG, "Error (%d) while doing finishing routine", err);
  }
}

void TestManager::run_tests()
{

  for (GenericTest *test : this->tests)
  {
  }
}

void TestManager::finish()
{
}