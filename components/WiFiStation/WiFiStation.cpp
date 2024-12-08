#include "WiFiStation.h"

#include "esp_check.h"
#include <cstring>
#include <algorithm>

static constexpr uint16_t DEFAULT_MAX_RETRY_NUM{5u};
static constexpr EventBits_t WIFI_CONNECTED_BIT{BIT0};
static constexpr EventBits_t WIFI_FAIL_BIT{BIT1};
static uint16_t retryNum{};

WiFiStation::State WiFiStation::state{State::NOT_INITIALIZED};
EventGroupHandle_t WiFiStation::eventGroup{};
wifi_init_config_t WiFiStation::initConfig = WIFI_INIT_CONFIG_DEFAULT();
wifi_config_t WiFiStation::config{};
uint16_t WiFiStation::maxRetryNum{DEFAULT_MAX_RETRY_NUM};
std::string WiFiStation::tag{"WiFiStation"};

WiFiStation::WiFiStation(const char *ssid, const char *password)
{
    setAuthMode(WIFI_AUTH_WPA2_PSK);
    setCredentials(ssid, password);
}

WiFiStation::WiFiStation(const std::string &ssid, const std::string &password)
{
    setAuthMode(WIFI_AUTH_WPA2_PSK);
    setCredentials(ssid, password);
}

esp_err_t WiFiStation::init()
{
    // Check if the WiFi station is not already initialized
    ESP_RETURN_ON_FALSE(state == State::NOT_INITIALIZED, ESP_FAIL, tag.c_str(), "already initialized");

    // Create an event group to signal in this method if the connection is established or not
    eventGroup = xEventGroupCreate();

    // Initialize the TCP/IP stack and create an event loop
    ESP_RETURN_ON_ERROR(esp_netif_init(), tag.c_str(), "failed to initialize the TCP/IP stack");
    ESP_RETURN_ON_ERROR(esp_event_loop_create_default(), tag.c_str(), "failed to create an event loop");
    esp_netif_create_default_wifi_sta();

    ESP_RETURN_ON_ERROR(esp_wifi_init(&initConfig), tag.c_str(), "failed to initialize the WiFi");

    // Register a handler for the WiFi events
    ESP_RETURN_ON_ERROR(esp_event_handler_instance_register(WIFI_EVENT,
                                                            ESP_EVENT_ANY_ID,
                                                            &wifiEventHandler,
                                                            nullptr,
                                                            nullptr), tag.c_str(), "failed to register the WiFi event handler");

    // Register a handler for the IP events
    ESP_RETURN_ON_ERROR(esp_event_handler_instance_register(IP_EVENT,
                                                            ESP_EVENT_ANY_ID,
                                                            &ipEventHandler,
                                                            nullptr,
                                                            nullptr), tag.c_str(), "failed to register the IP event handler");

    // Set WiFi to the station mode
    ESP_RETURN_ON_ERROR(esp_wifi_set_mode(WIFI_MODE_STA), tag.c_str(), "failed to set the station mode");

    // Set WiFi configuration
    ESP_RETURN_ON_ERROR(esp_wifi_set_config(WIFI_IF_STA, &config), tag.c_str(), "failed to set the configuration");
    ESP_RETURN_ON_ERROR(esp_wifi_start(), tag.c_str(), "failed to start the WiFi");

    // Change the state to indicate that the initialization is complete
    state = State::INITIALIZED;

    ESP_LOGI(tag.c_str(), "successfully initialized");

    // Wait until either the connection is established or connection failed
    EventBits_t bits = xEventGroupWaitBits(eventGroup,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(tag.c_str(), "successfully connected to the AP");
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        ESP_LOGE(tag.c_str(), "failed to connect to the AP");
    }
    else
    {
        ESP_LOGE(tag.c_str(), "UNEXPECTED EVENT");
    }

    return ESP_OK;
}

void WiFiStation::setCredentials(const char *ssid, const char *password)
{
    // Copy WiFi SSID and password to the config structure
    memcpy(config.sta.ssid, ssid, std::min(strlen(ssid), sizeof(config.sta.ssid)));
    memcpy(config.sta.password, password, std::min(strlen(password), sizeof(config.sta.password)));
}

void WiFiStation::setCredentials(const std::string &ssid, const std::string &password)
{
    setCredentials(ssid.c_str(), password.c_str());
}

void WiFiStation::setAuthMode(const wifi_auth_mode_t &authMode)
{
    config.sta.threshold.authmode = authMode;
}

void WiFiStation::setConnectMaxRetryNum(const uint16_t &maxRetryNum)
{
    WiFiStation::maxRetryNum = maxRetryNum;
}

void WiFiStation::setTag(const std::string &tag)
{
    WiFiStation::tag = tag;
}

const std::string &WiFiStation::getTag()
{
    return tag;
}

void WiFiStation::wifiEventHandler(void *arg, esp_event_base_t eventBase, int32_t eventID, void *eventData)
{
    if (WIFI_EVENT == eventBase)
    {
        const wifi_event_t event_type{static_cast<wifi_event_t>(eventID)};

        switch (event_type)
        {
        case WIFI_EVENT_STA_START:
        {
            esp_wifi_connect();

            break;
        }
        case WIFI_EVENT_STA_DISCONNECTED:
        {
            if(retryNum < maxRetryNum)
            {
                // Retry the connection
                esp_wifi_connect();
                retryNum++;
            }
            else
            {
                // Set the bit to indicate that the connection failed
                xEventGroupSetBits(eventGroup, WIFI_FAIL_BIT);
            }

            state = State::DISCONNECTED;

            break;
        }
        default:
            break;
        }
    }
}

void WiFiStation::ipEventHandler(void *arg, esp_event_base_t eventBase, int32_t eventID, void *eventData)
{
    if (IP_EVENT == eventBase)
    {
        const ip_event_t event_type{static_cast<ip_event_t>(eventID)};

        switch (event_type)
        {
        case IP_EVENT_STA_GOT_IP:
        {
            // Get IP address
            const ip_event_got_ip_t *event{static_cast<ip_event_got_ip_t *>(eventData)};
            ESP_LOGI(tag.c_str(), "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
            retryNum = 0;

            // Set the bit to indicate that the connection is established
            xEventGroupSetBits(eventGroup, WIFI_CONNECTED_BIT);

            state = State::CONNECTED;

            break;
        }
        case IP_EVENT_STA_LOST_IP:
        {
            ESP_LOGI(tag.c_str(), "lost ip");

            state = State::DISCONNECTED;

            break;
        }
        default:
            break;
        }
    }
}
