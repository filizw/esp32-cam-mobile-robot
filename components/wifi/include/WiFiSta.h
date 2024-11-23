#pragma once

#include "esp_wifi.h"
#include "esp_event.h"
#include <string>

class WiFiSta
{
public:
    enum class State
    {
        NOT_INITIALIZED,
        INITIALIZED,
        CONNECTED,
        DISCONNECTED
    };

    WiFiSta(const char *ssid, const char *password);
    WiFiSta(const std::string &ssid, const std::string &password);

    static void setCredentials(const char *ssid, const char *password);
    static void setCredentials(const std::string &ssid, const std::string &password);
    static void setAuthMode(const wifi_auth_mode_t &authMode);
    static void setConnectMaxRetryNum(const uint16_t &maxRetryNum);

    esp_err_t init();
private:
    static State state;
    static EventGroupHandle_t eventGroup;
    static wifi_init_config_t initConfig;
    static wifi_config_t config;
    static uint16_t maxRetryNum;

    static void wifiEventHandler(void *arg, esp_event_base_t eventBase, int32_t eventID, void *eventData);
    static void ipEventHandler(void *arg, esp_event_base_t eventBase, int32_t eventID, void *eventData);
};
