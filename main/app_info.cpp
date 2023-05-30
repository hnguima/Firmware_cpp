#include "app_info.hpp"

#include <esp_log.h>

// #include <driver/gpio.h>
#include "esp_event_base.h"

#include <esp_app_desc.h>
#include <esp_sntp.h>

static const char *TAG = "app_info";
esp_event_base_t SAFE_RESET_EVENT_BASE = "SAFE_RESET_EVENT_BASE";

void app_info_task(void *param)
{
  Settings<MainConfig> *settings = (Settings<MainConfig> *)param;

  // TODO: set hw version on the thing (???)
  const esp_app_desc_t *app_desc = esp_app_get_description();
  strcpy(settings->obj->firmware.fw_version, app_desc->version);
  sprintf(settings->obj->firmware.fw_timestamp, "%s, %s", app_desc->date, app_desc->time);

  settings->obj->general.resets++;
  settings->save();

  bool is_initialized = false;
  settings->obj->general.boot_time = time(NULL);

  while (true)
  {

    time_t curr_time = time(NULL);

    if (is_initialized == false && sntp_get_sync_status() == SNTP_SYNC_STATUS_COMPLETED)
    {

      if (settings->obj->general.install_time == 0)
      {
        settings->obj->general.install_time = curr_time;

        settings->save();
      }

      settings->obj->general.boot_time = curr_time;

      is_initialized = true;
    }

    settings->obj->general.up_time = curr_time - settings->obj->general.boot_time;

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

esp_err_t init_app_info_updater(Settings<MainConfig> *settings)
{

  xTaskCreate(app_info_task, "app_info_task", 4096, (void *)settings, 12, NULL);
  return ESP_OK;
}

void safe_reset_event_handler(void *handler_arg, esp_event_base_t base, int32_t id, void *event_data)
{
  ESP_LOGI(TAG, "safe reset event called");

  // Settings<MainConfig> *settings = (Settings<MainConfig> *)handler_arg;

  // settings->save();

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  esp_restart();
}

esp_event_loop_handle_t safe_reset_event_loop_handle;

void init_safe_reset_event_loop(Settings<MainConfig> *settings)
{
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  ESP_ERROR_CHECK(esp_event_handler_instance_register(SAFE_RESET_EVENT_BASE, SAFE_RESET_EVENT, safe_reset_event_handler, settings, NULL));
}