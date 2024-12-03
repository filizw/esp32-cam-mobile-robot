#pragma once

#include "esp_http_server.h"
#include <string>

class HTTPServer
{
public:
    HTTPServer();
    HTTPServer(const uint16_t &port);

    void setPort(const uint16_t &port);
    const uint16_t &getPort() const;

    esp_err_t start();
    esp_err_t stop();
    esp_err_t registerURIHandler(const std::string &uri, httpd_method_t method, esp_err_t (*handler)(httpd_req_t *));
private:
    httpd_handle_t handle{nullptr};
    uint16_t port{};
    bool isRunning{};
    std::string tag;
};
