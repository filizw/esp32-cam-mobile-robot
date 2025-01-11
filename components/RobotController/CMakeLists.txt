idf_component_register(SRCS "RobotController.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES WiFiStation HTTPServer CameraServer nvs_flash)
