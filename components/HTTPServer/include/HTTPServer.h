#pragma once

#include "esp_http_server.h"
#include <string>

class HTTPServer
{
public:
    HTTPServer();
    HTTPServer(uint16_t port);

    void setPort(uint16_t port);
    void setTag(const std::string &tag);
    uint16_t getPort() const;
    const std::string &getTag() const;

    esp_err_t start();
    esp_err_t stop();
    esp_err_t registerURIHandler(const std::string &uri, httpd_method_t method, esp_err_t (*handler)(httpd_req_t *), void *ctx = NULL);
private:
    static const uint16_t defaultPort{80};

    httpd_handle_t handle{};
    uint16_t port{defaultPort};
    bool isRunning{};
    std::string tag{"HTTPServer"};
};
