#pragma once

#include <esp_err.h>
#include "esp_settings.hpp"
#include "main.pb.h"
#include "esp_event.h"

extern esp_event_base_t SAFE_RESET_EVENT_BASE;

enum
{
  SAFE_RESET_EVENT
};

esp_err_t init_app_info_updater(Settings<MainConfig> *settings);

void init_safe_reset_event_loop(Settings<MainConfig> *settings);
// esp_err_t init_settings_http_handler();
