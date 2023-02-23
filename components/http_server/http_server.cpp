#include "http_server.hpp"

#include <string>

// FreeRTOS
#include "freertos/FreeRTOS.h"

#include <esp_err.h>
#include <esp_log.h>

#include "esp_filesystem.hpp"

static const char *TAG = "HTTPServer";

HTTPServer *HTTPServer::_singleton = nullptr;

#define HTTP_MAX_BUFFER_SIZE 1024

static esp_err_t default_uri_handler(httpd_req_t *req)
{
  std::string uri(req->uri);

  ESP_LOGI(TAG, "Hello %s called method %d", uri.c_str(), req->method);

  // TODO: if uri = "/"  redirect to "/index"
  // TODO: search file in fs and proceed accordig to method

  std::string file_name = "/data";
  
  {
    // TODO: remember to move this to its own function
    if (uri == "/")
    {
      file_name += "/index.html";
    }
    else if (!(uri.find(".") != std::string::npos))
    {
      file_name += uri + ".html";
    }
    else
    {
      file_name += uri;
    }
  }

  {
    // TODO: remember to move this to its own function
    if (uri.find(".html") != std::string::npos)
    {
      httpd_resp_set_type(req, "text/html");
    }
    else if (uri.find(".js") != std::string::npos)
    {
      httpd_resp_set_type(req, "text/javascript");
    }
  }

  httpd_resp_set_hdr(req, "Content-Encoding", "gzip");

  FileSystem *fs = FileSystem::get_instance();

  // TODO: check if file exists
  int size_read = 0;

  do
  {
    char file_buffer[HTTP_MAX_BUFFER_SIZE] = {};
    size_read = fs->read(file_name.c_str(), file_buffer, HTTP_MAX_BUFFER_SIZE);

    // if (size_read)
    // {
    //   /* code */
    // }

    httpd_resp_send_chunk(req, file_buffer, size_read);
  } while (size_read == HTTP_MAX_BUFFER_SIZE);

  httpd_resp_send_chunk(req, NULL, 0);
  httpd_resp_set_status(req, HTTPD_200);

  return ESP_OK;
}

httpd_uri_t default_uri = {
    .uri = "/*",
    .method = HTTP_GET,
    .handler = default_uri_handler,
    .user_ctx = NULL,
};

HTTPServer *HTTPServer::get_instance()
{

  if (_singleton == nullptr)
  {
    _singleton = new HTTPServer();
  }
  return _singleton;
}

HTTPServer::HTTPServer()
{
  esp_err_t err = ESP_OK;

  this->handle = NULL;

  httpd_config_t http_config = HTTPD_DEFAULT_CONFIG();
  http_config.uri_match_fn = httpd_uri_match_wildcard;
  http_config.max_uri_handlers = 10;
  http_config.backlog_conn = 10;
  http_config.max_open_sockets = 3;
  http_config.lru_purge_enable = true;
  http_config.stack_size = 8192;

  err = ESP_ERROR_CHECK_WITHOUT_ABORT(httpd_start(&this->handle, &http_config));

  if (err)
  {
    ESP_LOGE(TAG, "Error(%s) starting http server...", esp_err_to_name(err));
    return;
  }

  err = ESP_ERROR_CHECK_WITHOUT_ABORT(httpd_register_uri_handler(this->handle, &default_uri));

  ESP_LOGI(TAG, "Server Started");
}

esp_err_t HTTPServer::register_uri(const httpd_uri_t *uri_handler)
{
  esp_err_t err = ESP_OK;

  err = ESP_ERROR_CHECK_WITHOUT_ABORT(httpd_unregister_uri_handler(this->handle, "/*", uri_handler->method));
  err = ESP_ERROR_CHECK_WITHOUT_ABORT(httpd_register_uri_handler(this->handle, uri_handler));

  default_uri.method = uri_handler->method;
  err = ESP_ERROR_CHECK_WITHOUT_ABORT(httpd_register_uri_handler(this->handle, &default_uri));

  return err;
}
