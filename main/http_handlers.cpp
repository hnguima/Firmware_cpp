#include "http_handlers.hpp"

#include <esp_log.h>
#include "http_server.hpp"

#include <driver/gpio.h>

static const char *TAG = "settings_handler";

esp_err_t get_settings_uri_handler(httpd_req_t *req);
esp_err_t put_settings_uri_handler(httpd_req_t *req);

esp_err_t get_settings_uri_handler(httpd_req_t *req)
{
  esp_err_t err = ESP_OK;

  ESP_LOGI(TAG, "get settings");

  Settings<MainConfig> *settings = (Settings<MainConfig> *)req->user_ctx;

  uint8_t *message;
  size_t message_length;

  settings->encode(&message, &message_length);

  httpd_resp_send(req, (char *)message, message_length);

  return err;
}

esp_err_t put_settings_uri_handler(httpd_req_t *req)
{
  esp_err_t err = ESP_OK;

  ESP_LOGI(TAG, "put settings");

  Settings<MainConfig> *settings = (Settings<MainConfig> *)req->user_ctx;

  uint8_t buffer[1024];

  size_t buffer_length = httpd_req_recv(req, (char *)buffer, 1024);

  settings->decode(buffer, buffer_length);
  settings->save();

  const char resp[] = "Success";
  httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);

  return err;
}

esp_err_t init_settings_http_handler(Settings<MainConfig> *settings)
{

  HTTPServer *server = HTTPServer::get_instance();

  httpd_uri_t get_settings_uri = {
      .uri = "/settings",
      .method = HTTP_GET,
      .handler = get_settings_uri_handler,
      .user_ctx = (void *)settings,
  };

  httpd_uri_t put_settings_uri = {
      .uri = "/settings",
      .method = HTTP_PUT,
      .handler = put_settings_uri_handler,
      .user_ctx = (void *)settings,
  };

  server->register_uri(&get_settings_uri);
  server->register_uri(&put_settings_uri);

  return ESP_OK;
}