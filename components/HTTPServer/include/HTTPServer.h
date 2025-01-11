#pragma once

#include "esp_http_server.h"
#include "esp_check.h"
#include <string>

class HTTPServer
{
public:
    /*
    Default constructor

    REMARKS: sets port number to 80
    */
    HTTPServer();
    /*
    Constructor: sets port number

    INPUT: port number
    */
    HTTPServer(uint16_t port);

    /*
    Sets port number

    INPUT: port number
    */
    void setPort(uint16_t port);
    /*
    Sets tag identifier for log messages

    INPUT: tag identifier
    */
    void setTag(const std::string &tag);

    /*
    Retrives current port number

    OUTPUT: port number
    */
    uint16_t getPort() const;
    /*
    Retrives current tag identifier for log messages

    OUTPUT: tag identifier
    */
    const std::string &getTag() const;

    /*
    Starts web server, can be overridden

    OUTPUT: error code (ESP_OK if there were no errors)

    REMARKS: virtual method, must run startServer method inside
    */
    virtual esp_err_t start();
    /*
    Stops web server, can be overridden

    OUTPUT: error code (ESP_OK if there were no errors)

    REMARKS: virtual method, must run stopServer method inside
    */
    virtual esp_err_t stop();
    /*
    Returns boolean value indicating if server is running

    OUTPUT: true if server is running, false otherwise
    */
    bool isRunning() const;
    /*
    Registers given URI handler

    INPUT: URI, HTTP method, pointer to URI handler, context(optional)

    OUTPUT: error code (ESP_OK if there were no errors)
    */
    esp_err_t registerURIHandler(const std::string &uri, httpd_method_t method, esp_err_t (*handler)(httpd_req_t *), void *ctx = NULL);

protected:
    /*
    Starts web server

    OUTPUT: error code (ESP_OK if there were no errors)
    */
    esp_err_t startServer();
    /*
    Stops web server

    OUTPUT: error code (ESP_OK if there were no errors)
    */
    esp_err_t stopServer();

private:
    httpd_handle_t handle{};
    uint16_t port{};
    std::string tag{};
    bool isServerRunning{};
};
