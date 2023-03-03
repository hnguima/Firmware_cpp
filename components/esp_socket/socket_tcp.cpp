#include "socket_tcp.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_log.h>

static const char *TAG = "Socket";

SocketClient::SocketClient()
{
  ESP_LOGI(TAG, "Initializing socket");
}

void socket_server_task(void *vParam)
{

  SocketClient *socket = (SocketClient *)vParam;

  int i = 0;

  while (true)
  {
    socket->on_connect_cb(i++);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

esp_err_t SocketClient::on_connect(recv_handler_func_t func)
{
  this->on_connect_cb = func;

  xTaskCreate(socket_server_task, "test", 4096, (void *)this, 5, NULL);

  return ESP_OK;
}
