#pragma once

#include "esp_http_server.h"
#include <string>

class HTTPServer
{
public:
    HTTPServer();
    HTTPServer(const uint16_t &port);

    void setPort(const uint16_t &port);
    void setTag(const std::string &tag);
    const uint16_t &getPort() const;
    const std::string &getTag() const;

    esp_err_t start();
    esp_err_t stop();
    esp_err_t registerURIHandler(const std::string &uri, httpd_method_t method, esp_err_t (*handler)(httpd_req_t *), void *ctx = NULL);
private:
    httpd_handle_t handle{nullptr};
    uint16_t port{80}; // Set default port
    bool isRunning{};
    std::string tag{"HTTPServer"}; // Set default tag for log messages
};
