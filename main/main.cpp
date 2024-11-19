#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "wifi.h"

#define LED_PIN GPIO_NUM_4

extern "C" void app_main(void)
{
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    func();

    int level = 0;

    while(1)
    {
        level = !level;
        gpio_set_level(LED_PIN, level);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
