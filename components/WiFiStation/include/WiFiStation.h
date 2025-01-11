#pragma once

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_check.h"
#include <string>
#include <cstring>
#include <algorithm>

class WiFiStation
{
public:
    // Possible states Wi-Fi station can be in
    enum class State
    {
        NOT_INITIALIZED,    // Wi-Fi station not initialized
        INITIALIZED,        // Wi-Fi station initialized
        CONNECTED,          // Wi-Fi station connected to network
        DISCONNECTED        // Wi-Fi station disconnected from network
    };

    /*
    Constructor: sets Wi-Fi credentials using C-style strings

    INPUT: Wi-Fi SSID and password

    REMARKS: sets Wi-Fi authentication mode to WPA2-PSK
    */
    WiFiStation(const char *ssid, const char *password);
    /*
    Constructor: sets Wi-Fi credentials using std::string

    INPUT: SSID and password

    REMARKS: sets authentication mode to WPA2-PSK
    */
    WiFiStation(const std::string &ssid, const std::string &password);

    /*
    Sets Wi-Fi credentials using C-style strings

    INPUT: SSID and password
    */
    static void setCredentials(const char *ssid, const char *password);
    /*
    Sets Wi-Fi credentials using std::string

    INPUT: SSID and password
    */
    static void setCredentials(const std::string &ssid, const std::string &password);
    /*
    Sets Wi-Fi authentication mode

    INPUT: authentication mode
    */
    static void setAuthMode(wifi_auth_mode_t authMode);
    /*
    Sets maximum number of Wi-Fi connection retry attempts

    INPUT: maximum number of retry attempts
    */
    static void setConnectMaxRetryNum(uint16_t maxRetryNum);
    /*
    Sets static IPv4 address, gateway and subnet mask for Wi-Fi station

    INPUT: IPv4 address, gateway and subnet mask
    */
    static void setAddress(const std::string& address, const std::string& gateway, const std::string& subnetMask);
    /*
    Sets tag identifier for log messages

    INPUT: tag identifier
    */
    static void setTag(const std::string &tag);
    
    /*
    Retrives current tag identifier for log messages

    OUTPUT: tag identifier
    */
    static const std::string &getTag();

    /*
    Initializes Wi-Fi station and sets all necessarry configurations

    OUTPUT: error code (ESP_OK if initialization was successfull)

    REMARKS: registers Wi-Fi and IP event handlers
    */
    esp_err_t init();
private:
    static State state; // Current Wi-Fi station state
    static EventGroupHandle_t eventGroup;
    static wifi_init_config_t initConfig;
    static wifi_config_t config;
    static uint16_t maxRetryNum; // Maximum number of connection retry attempts
    static std::string address;
    static std::string gateway;
    static std::string subnetMask;
    static std::string tag;

    /*
    Handles Wi-Fi related events

    INPUT: specified by ESP-IDF API
    */
    static void wifiEventHandler(void *arg, esp_event_base_t eventBase, int32_t eventID, void *eventData);
    /*
    Handles IP related events

    INPUT: specified by ESP-IDF API
    */
    static void ipEventHandler(void *arg, esp_event_base_t eventBase, int32_t eventID, void *eventData);
};
