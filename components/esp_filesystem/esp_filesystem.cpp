#include "esp_filesystem.hpp"

#include <freertos/FreeRTOS.h>

#include <esp_log.h>

#include <esp_partition.h>

static const char *TAG = "FileSystem";

FileSystem *FileSystem::_singleton = nullptr;

FileSystem::FileSystem()
{
  esp_partition_iterator_t partition_iterator = esp_partition_find(
      ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_SPIFFS, NULL);

  const esp_partition_t *partition = NULL;

  do
  {
    partition = esp_partition_get(partition_iterator);

    ESP_LOGI(TAG, "Found partition: %s", partition->label);

    char *base_path = (char *)malloc(sizeof(char) * 32);
    base_path[0] = '/';
    strcpy(base_path + 1, partition->label);

    esp_vfs_spiffs_conf_t spiffs_config = {
        .base_path = base_path,
        .partition_label = partition->label,
        .max_files = 1,
        .format_if_mount_failed = false,
    };

    this->partitions.push_back(spiffs_config);

    partition_iterator = esp_partition_next(partition_iterator);
  } while (partition_iterator != NULL);

  this->file = nullptr;
}

FileSystem *FileSystem::get_instance()
{

  if (_singleton == nullptr)
  {
    _singleton = new FileSystem();
  }
  return _singleton;
}

esp_err_t FileSystem::mount(const char *base_path)
{
  esp_err_t err = ESP_OK;

  for (esp_vfs_spiffs_conf_t config : this->partitions)
  {

    if (strcmp(config.base_path, base_path) == 0)
    {
      err = esp_vfs_spiffs_register(&config);

      if (err == ESP_FAIL)
      {
        ESP_LOGE(TAG, "Failed to mount or format filesystem");
        return err;
      }
      if (err == ESP_ERR_NOT_FOUND)
      {
        ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        return err;
      }
      if (err != ESP_OK)
      {
        ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(err));
        return err;
      }

      ESP_LOGI(TAG, "Mounted: %s", config.base_path);
      return err;
    }
  }

  ESP_LOGE(TAG, "Failed to find SPIFFS partition (%s)", base_path);
  return err;
}

esp_err_t FileSystem::mount_all()
{
  esp_err_t err = ESP_OK;

  for (esp_vfs_spiffs_conf_t config : this->partitions)
  {
    err = esp_vfs_spiffs_register(&config);

    if (err == ESP_FAIL)
    {
      ESP_LOGE(TAG, "Failed to mount or format filesystem");
      return err;
    }
    if (err == ESP_ERR_NOT_FOUND)
    {
      ESP_LOGE(TAG, "Failed to find SPIFFS partition");
      return err;
    }
    if (err != ESP_OK)
    {
      ESP_LOGE(TAG, "Failed to initialize SPIFFS (%d)", err);
      return err;
    }

    ESP_LOGI(TAG, "Mounted: %s", config.base_path);
  }

  return err;
}

esp_err_t FileSystem::unmount(const char *base_path)
{
  esp_err_t err = ESP_OK;

  for (esp_vfs_spiffs_conf_t config : this->partitions)
  {
    if (strcmp(config.base_path, base_path) == 0)
    {
      err = esp_vfs_spiffs_unregister(config.base_path);
      if (err != ESP_OK)
      {
        ESP_LOGE(TAG, "Failed unmounting: %s", config.base_path);
        return err;
      }

      ESP_LOGI(TAG, "Unmounted: %s", config.base_path);
      return err;
    }
  }

  ESP_LOGE(TAG, "Failed to find SPIFFS partition (%d)", err);
  return err;
}

esp_err_t FileSystem::unmount_all()
{
  esp_err_t err = ESP_OK;

  for (esp_vfs_spiffs_conf_t config : this->partitions)
  {
    err = esp_vfs_spiffs_unregister(config.base_path);
    ESP_LOGI(TAG, "Unmounted: %s", config.base_path);
  }

  return err;
}

size_t FileSystem::read(const char *file_name, char *output, size_t size)
{

  if (this->find(file_name) != ESP_OK)
  {
    return 0;
  }

  // Read 'size' bytes:
  size_t read_size = fread(output, 1, size, this->file);

  if (feof(this->file))
  {
    fclose(this->file);
    this->file = NULL;
  }

  return read_size;
}

esp_err_t FileSystem::find(const char *file_name)
{

  if (this->file == NULL)
  {
    this->file = fopen(file_name, "r");

    if (this->file == NULL)
    {
      ESP_LOGE(TAG, "File not found: %s", file_name);
      return ESP_ERR_NOT_FOUND;
    }
  }

  return ESP_OK;
}

esp_err_t FileSystem::abort()
{

  if (this->file == NULL)
  {
    ESP_LOGE(TAG, "File stream not open");
    return ESP_FAIL;
  }

  fclose(this->file);
  this->file = NULL;

  return ESP_OK;
}