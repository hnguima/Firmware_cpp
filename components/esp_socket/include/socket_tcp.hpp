#pragma once

#include <iostream>
#include <functional>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <esp_err.h>

#include <esp_log.h>

class SocketClient
{
protected:
public:
    SocketClient();

    // // /**
    // //  * @brief adiciona o grupo de eventos a ser observado pelo socket, principalmente o evento de conexão
    // //  *
    // //  * @param event_group grupo de eventos a ser associado ao socket
    // //  * @return esp_err_t
    // //  */
    // // esp_err_t socket_manager_init(EventGroupHandle_t *event_group);

    // /**
    //  * @brief adiciona conteudo a ser enviado pelo socket, assim que a tas estiver disponpivel o dado
    //  * será enviado
    //  *
    //  * @param socket ponteiro para estrutura do socket
    //  * @param buffer buffer a ser enviado
    //  * @param size tamanho do dado a ser enviado
    //  * @return esp_err_t retorna erro caso o socket não esteja aberto ou se o tamanho do dado for
    //  *           maior que o suportado
    //  */
    // esp_err_t send(socket_ctx_t *socket, uint8_t *buffer, uint16_t size);

    typedef std::function<void(void)> handler_func_t;
    typedef std::function<void(int)> recv_handler_func_t;

    esp_err_t on_connect(recv_handler_func_t func);
    esp_err_t on_disconnect(handler_func_t func);
    esp_err_t on_recv(handler_func_t func);

    // esp_err_t socket_task_create(socket_ctx_t *new_socket, uint8_t type);

    // /**
    //  * @brief deleta a task associada ao socket, fechando a conexão
    //  *
    //  * @param socket descritor do socket a ser fechado
    //  * @return esp_err_t retorna o erro que causou o fechamento da conexão
    //  */
    // esp_err_t socket_task_delete(int socket);

    recv_handler_func_t on_connect_cb;
    handler_func_t on_disconnect_cb;
    handler_func_t recv_cb;

private:
    // in_addr_t addr;
    // in_port_t port;

    // int cli_sock;

    // uint8_t buf_rx[SOCKET_RX_BUF_MAX];
    // int16_t size_rx;
    // uint8_t buf_tx[SOCKET_TX_BUF_MAX];
    // int16_t size_tx;

    // void (*recv_cb)(uint8_t *data_rx, int16_t size_rx);
    // void (*on_disconnect_cb)();

    // bool is_persistent;
    // uint8_t retry_max;
    // uint16_t retry_delay;
    // uint8_t retry;

    // SemaphoreHandle_t mutex;
};
