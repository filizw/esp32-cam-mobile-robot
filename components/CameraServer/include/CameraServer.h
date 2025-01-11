#pragma once

#include "HTTPServer.h"
#include "esp_camera.h"
#include "esp_log.h"

class CameraServer : public HTTPServer
{
public:
     /*
    Default constructor

    REMARKS: sets port number to 80
    */
    CameraServer();
    /*
    Constructor: sets port number

    INPUT: port number
    */
    CameraServer(uint16_t port);

    /*
    Starts web server and initializes camera

    OUTPUT: error code (ESP_OK if there were no errors)

    REMARKS: registers JPEG stream URI handler(URI = /stream)
    */
    esp_err_t start() override;

private:
    /*
    Initializes camera

    OUTPUT: error code (ESP_OK if there were no errors)

    REMARKS: registers JPEG stream URI handler(URI = /stream)
    */
    static esp_err_t cameraInit();
    /*
    Handles JPEG stream

    INPUT: HTTP request

    OUTPUT: error code (ESP_OK if there were no errors)

    REMARKS: specified by ESP-IDF API
    */
    static esp_err_t jpgStreamHandler(httpd_req_t *req);
};
