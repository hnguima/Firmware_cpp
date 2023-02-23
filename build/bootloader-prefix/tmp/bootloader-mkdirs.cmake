# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Espressif/frameworks/esp-idf-v5.0/components/bootloader/subproject"
  "C:/Users/hngui/OneDrive/Documentos/Impressora3d/Firmware_cpp/build/bootloader"
  "C:/Users/hngui/OneDrive/Documentos/Impressora3d/Firmware_cpp/build/bootloader-prefix"
  "C:/Users/hngui/OneDrive/Documentos/Impressora3d/Firmware_cpp/build/bootloader-prefix/tmp"
  "C:/Users/hngui/OneDrive/Documentos/Impressora3d/Firmware_cpp/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/hngui/OneDrive/Documentos/Impressora3d/Firmware_cpp/build/bootloader-prefix/src"
  "C:/Users/hngui/OneDrive/Documentos/Impressora3d/Firmware_cpp/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/hngui/OneDrive/Documentos/Impressora3d/Firmware_cpp/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/hngui/OneDrive/Documentos/Impressora3d/Firmware_cpp/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
