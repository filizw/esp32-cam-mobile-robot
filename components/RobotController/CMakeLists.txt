idf_component_register(SRCS "RobotController.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES WiFiStation HTTPServer nvs_flash esp_driver_gpio)
