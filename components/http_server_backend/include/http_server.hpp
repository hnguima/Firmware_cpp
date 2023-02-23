#pragma once

#include <esp_http_server.h>

class HTTPServer
{
protected:
  static HTTPServer *_singleton;
  HTTPServer();

public:
  static HTTPServer *get_instance();
  esp_err_t register_uri(const httpd_uri_t *uri);

  HTTPServer(HTTPServer &other) = delete;
  void operator=(const HTTPServer &) = delete;

private:
  httpd_handle_t handle;
};
