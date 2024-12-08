#pragma once

#include "esp_wifi.h"
#include "esp_event.h"
#include <string>

class WiFiStation
{
public:
    enum class State
    {
        NOT_INITIALIZED,
        INITIALIZED,
        CONNECTED,
        DISCONNECTED
    };

    WiFiStation(const char *ssid, const char *password);
    WiFiStation(const std::string &ssid, const std::string &password);

    esp_err_t init();

    static void setCredentials(const char *ssid, const char *password);
    static void setCredentials(const std::string &ssid, const std::string &password);
    static void setAuthMode(const wifi_auth_mode_t &authMode);
    static void setConnectMaxRetryNum(const uint16_t &maxRetryNum);
    static void setTag(const std::string &tag);
    static const std::string &getTag();
private:
    static State state;
    static EventGroupHandle_t eventGroup;
    static wifi_init_config_t initConfig;
    static wifi_config_t config;
    static uint16_t maxRetryNum;
    static std::string tag;

    static void wifiEventHandler(void *arg, esp_event_base_t eventBase, int32_t eventID, void *eventData);
    static void ipEventHandler(void *arg, esp_event_base_t eventBase, int32_t eventID, void *eventData);
};
