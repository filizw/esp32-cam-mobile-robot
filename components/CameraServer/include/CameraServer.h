#pragma once

#include "HTTPServer.h"
#include "esp_camera.h"
#include "esp_log.h"

class CameraServer : public HTTPServer
{
public:
    CameraServer();
    CameraServer(uint16_t port);

    esp_err_t start() override;

private:
    static const char *CAMERA_TAG;

    static void cameraInit();
    static esp_err_t jpgStreamHandler(httpd_req_t *req);
};
