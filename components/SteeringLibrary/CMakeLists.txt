idf_component_register(SRCS "src/Vehicle4WD.cpp" "src/Vehicle4WDSteering.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES driver freertos esp_rom soc)