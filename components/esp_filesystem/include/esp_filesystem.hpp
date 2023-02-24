#pragma once

#include <vector>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <esp_err.h>
#include <esp_spiffs.h>

class FileSystem
{
protected:
  static FileSystem *_singleton;
  FileSystem();

public:
  std::vector<esp_vfs_spiffs_conf_t> partitions;

  esp_err_t mount(const char *base_path);
  esp_err_t mount_all();
  esp_err_t unmount(const char *base_path);
  esp_err_t unmount_all();

  esp_err_t find(const char *file_name);
  size_t read(const char *file_name, char *output, size_t size);
  esp_err_t abort();

  static FileSystem *get_instance();

  FileSystem(FileSystem &other) = delete;
  void operator=(const FileSystem &) = delete;

private:
  FILE *file;
};
