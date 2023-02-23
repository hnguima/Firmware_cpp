# # Menor versão com suporte a cross-compiling
cmake_minimum_required(VERSION 3.16)

idf_component_register(SRCS 
                        "http_server.cpp"
                    INCLUDE_DIRS   
                        "include"
                    REQUIRES 
                        esp_http_server
                        esp_filesystem
                         )
                        
                    