#include "HTTPServer.h"

const std::string HTTPServer::DEFAULT_TAG{"HTTPServer"};

HTTPServer::HTTPServer()
{

}

HTTPServer::HTTPServer(uint16_t port) : port(port)
{

}

void HTTPServer::setPort(uint16_t port)
{
    this->port = port;
}

void HTTPServer::setTag(const std::string &tag)
{
    this->tag = tag;
}

uint16_t HTTPServer::getPort() const
{
    return port;
}

const std::string &HTTPServer::getTag() const
{
    return tag;
}

esp_err_t HTTPServer::start()
{
    return startServer();
}

esp_err_t HTTPServer::stop()
{
    return stopServer();   
}

bool HTTPServer::isRunning() const
{
    return isServerRunning;
}

esp_err_t HTTPServer::registerURIHandler(const std::string &uri, httpd_method_t method, esp_err_t (*handler)(httpd_req_t *), void *ctx)
{
    ESP_RETURN_ON_FALSE(isServerRunning, ESP_ERR_INVALID_STATE, tag.c_str(), "server is not running");

    httpd_uri_t uriHandler = {
        .uri = uri.c_str(),
        .method = method,
        .handler = handler,
        .user_ctx = ctx
    };

    ESP_RETURN_ON_ERROR(httpd_register_uri_handler(handle, &uriHandler), tag.c_str(),
                        "failed to register handler for the URI: \"%s\"", uri.c_str());

    return ESP_OK;
}

esp_err_t HTTPServer::startServer()
{
    ESP_RETURN_ON_FALSE(!isServerRunning, ESP_ERR_INVALID_STATE, tag.c_str(), "server is already running");

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = port;
    config.ctrl_port = ESP_HTTPD_DEF_CTRL_PORT + port - DEFAULT_PORT; // Change control port
    config.max_uri_handlers = 16;

    // Start the web server
    ESP_RETURN_ON_ERROR(httpd_start(&handle, &config), tag.c_str(), "failed to start the server");

    isServerRunning = true;
    ESP_LOGI(tag.c_str(), "successfully started the server");

    return ESP_OK;
}

esp_err_t HTTPServer::stopServer()
{
    ESP_RETURN_ON_FALSE(isServerRunning, ESP_ERR_INVALID_STATE, tag.c_str(), "server is not running");

    // Stop the web server
    ESP_RETURN_ON_ERROR(httpd_stop(handle), tag.c_str(), "failed to stop the server");

    handle = nullptr;
    isServerRunning = false;
    ESP_LOGI(tag.c_str(), "stopped the server");

    return ESP_OK;
}
