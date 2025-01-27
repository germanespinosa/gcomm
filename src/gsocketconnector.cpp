#include "gcomm.h"
#include <unistd.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <iostream>

namespace gcomm {
    GSocketConnector::GSocketConnector(std::string remote, uint16_t port) {
        connected = false;
        struct sockaddr_in serv_addr;
        if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            exit(EXIT_FAILURE);
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);

        if (inet_pton(AF_INET, remote.c_str(), &serv_addr.sin_addr) <= 0) {
            exit(EXIT_FAILURE);
        }

        if (connect(_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            exit(EXIT_FAILURE);
        }
        connected = true;
    }

    GSocketConnector::GSocketConnector(uint16_t port) {
        connected = false;
        _connect = std::thread(_connect_server, std::ref(*this), port);

    }

    void GSocketConnector::_connect_server(GSocketConnector &server, uint16_t port) {
        struct sockaddr_in address;
        int fd;
        int opt = 1;
        if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            exit(EXIT_FAILURE);
        }

        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (bind(fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
            exit(EXIT_FAILURE);
        }

        if (listen(fd, 3) < 0) {
            exit(EXIT_FAILURE);
        }

        int addrlen = sizeof(address);

        if ((server._socket = accept(fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
            exit(EXIT_FAILURE);
        }
        server.connected = true;

    }


    uint16_t GSocketConnector::read_buffer(void *buffer, uint16_t max_size) {
        return read(_socket, buffer, max_size);
    }

    void GSocketConnector::write_buffer(const void *buffer, uint16_t size) {
        send(_socket, buffer, size, 0);
    }
}