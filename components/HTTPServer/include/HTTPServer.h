#pragma once

#include "esp_http_server.h"
#include "esp_check.h"
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

    virtual esp_err_t start();
    virtual esp_err_t stop();
    bool isRunning() const;
    esp_err_t registerURIHandler(const std::string &uri, httpd_method_t method, esp_err_t (*handler)(httpd_req_t *), void *ctx = NULL);

protected:
    static const uint16_t DEFAULT_PORT{80};
    static const std::string DEFAULT_TAG;

    esp_err_t startServer();
    esp_err_t stopServer();

private:
    httpd_handle_t handle{};
    uint16_t port{DEFAULT_PORT};
    std::string tag{DEFAULT_TAG};
    bool isServerRunning{};
};
