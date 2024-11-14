
idf_component_register(SRCS "main.c" "wifi_handler.c"
                        INCLUDE_DIRS .
                        REQUIRES nvs_flash esp_hw_support mdns esp_driver_gpio esp_wifi proj-mdns)

