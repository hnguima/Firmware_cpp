#pragma once

#include <string>

#include <esp_err.h>
#include <driver/uart.h>

class GenericTest
{
}

class HardwareTest : public GenericTest
{
}

class IntegrationTest : public GenericTest
{
}

class UnitTest : public GenericTest
{
}

class TestManager
{

public:
  typedef std::function<void(void)> handler_func_t;

  TestManager();
  TestManager(handler_func_t setup);
  TestManager(handler_func_t setup, handler_func_t finish);

  void set_setup_func(handler_func_t setup);
  void set_finish_func(handler_func_t finish);

  void start();
  void run_tests();
  void finish();

private:
  handler_func_t setup;
  handler_func_t finish;

  std::vector<GenericTest *> tests;
}
