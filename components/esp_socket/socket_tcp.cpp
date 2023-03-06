#include "socket_tcp.hpp"

#include <vector>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_log.h>
#include <esp_event.h>
#include <esp_netif.h>

static const char *TAG = "Socket";

bool has_network = false;

static void socket_event_handler(void *arg, esp_event_base_t event_base,
                                 int32_t event_id, void *event_data);

Socket::Socket() : on_recv_cb(NULL),
                   is_persistent(true),
                   is_vital(false),
                   retries(0),
                   max_retries(0),
                   retry_delay(SOCKET_RETRY_INTERVAL)
{

  if (!(this->open_sockets.size() < MAX_SOCKETS))
  {
    ESP_LOGE(TAG, "Numero de servers abertos ja é o maximo permitido: %d", MAX_SOCKETS);
    return;
  }

  for (Socket *socket : this->open_sockets)
  {
    if (this == socket)
    {
      ESP_LOGE(TAG, "Já existe um servidor aberto com este IP e porta");
      return;
    }
  }

  this->mutex = xSemaphoreCreateMutex();

  if (this->mutex == NULL)
  {
    ESP_LOGE(TAG, "nao foi possivel criar o mutex");
    return;
  }

  ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_loop_create_default());

  esp_event_handler_instance_t instance_got_ip;
  ESP_ERROR_CHECK_WITHOUT_ABORT(esp_event_handler_instance_register(IP_EVENT,
                                                                    ESP_EVENT_ANY_ID,
                                                                    &socket_event_handler,
                                                                    this,
                                                                    &instance_got_ip));
}

void Socket::delete_task()
{
  // Falha na abertura do socket Server, ou por falta de memória ou IP errado do servidor
  ESP_LOGI(TAG, "deleging socket task...");

  esp_err_t err = close(this->fd);
  ESP_LOGI(TAG, "error (%d) closing the socket", err);
  perror("close");

  // Deleting the current task
  vTaskDelete(*this->task_handle);

  this->open_sockets.erase(
      std::remove(this->open_sockets.begin(), this->open_sockets.end(), this),
      this->open_sockets.end());
}

bool Socket::recv_loop()
{
  while (true)
  {
    this->size_rx = recv(this->fd, this->buffer_rx, 1024, 0);

    if (this->size_rx <= 0)
    {
      ESP_LOGE(this->task_name, "error (%d) on recv", errno);
      perror("recv");

      // Deleta a task atual da lista de tasks monitoradas por watchdog
      //  esp_task_wdt_delete(NULL);

      // break the receiving loop and retry connection;

      RETURN_OR_BREAK(this->retry() == 0);
    }

    this->retries = 0;

    // ESP_LOG_BUFFER_HEXDUMP(this->task_name, this->buffer_rx, this->size_rx, ESP_LOG_WARN);

    this->buffer_rx[this->size_rx] = '\0';
    if (this->on_recv_cb)
    {
      this->on_recv_cb(this->buffer_rx, this->size_rx);
    }

    // Reseta o watchdog, caso não tenha travado
    //  esp_task_wdt_reset();
    //  Bloqueia a task por alguns instantes.
    //  Caso seja necessario maior throughput de dados, diminua esse intervalo.
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }

  return true;
}

bool Socket::retry()
{

  if (this->is_persistent)
  {

    ESP_LOGW(this->task_name, "retrying in %.3fs, retry attempt: %d.", (float)this->retry_delay / 1000, ++this->retries);
    vTaskDelay(this->retry_delay / portTICK_PERIOD_MS);

    return true;
  }

  if (this->retries < this->max_retries)
  {

    ESP_LOGW(this->task_name, "retrying in %.3fs, retry attempt: %d of %d.", (float)this->retry_delay / 1000, ++this->retries, this->max_retries);
    vTaskDelay(this->retry_delay / portTICK_PERIOD_MS);

    return true;
  }

  this->delete_task();

  if (this->is_vital)
  {
    ESP_LOGE(this->task_name, "could not connect, max retries achieved. Restarting device in %ds...", 2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    ESP_LOGI(this->task_name, "restarting...");

    esp_restart();
  }

  return false;
}

void Socket::wait_for_network()
{

  while (has_network == false)
  {
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

esp_err_t Socket::on_recv(recv_handler_func_t func)
{
  this->on_recv_cb = func;
  return ESP_OK;
}

static void socket_event_handler(void *arg, esp_event_base_t event_base,
                                 int32_t event_id, void *event_data)
{

  if (event_id == IP_EVENT_STA_GOT_IP ||
      event_id == IP_EVENT_GOT_IP6 ||
      event_id == IP_EVENT_ETH_GOT_IP ||
      event_id == IP_EVENT_PPP_GOT_IP)
  {
    has_network = true;
  }

  else if (event_id == IP_EVENT_STA_LOST_IP ||
           event_id == IP_EVENT_ETH_LOST_IP ||
           event_id == IP_EVENT_PPP_LOST_IP)
  {
    has_network = false;
  }
}