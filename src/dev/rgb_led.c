#include "rgb_led.h"
#include "../sys/ledrgb.h"

void RGB_LED_SET(color_t* colors) {
    bool tab[193] = { false };
    uint8_t index = 0;
    for (uint8_t i = 0; i < RGB_LED_NUM; i++) { // for each led
        color_t color = colors[i];
        uint8_t components[3] = {color.g, color.r, color.b};
        for (uint8_t j = 0; j < 3; j++) { // for each component
            uint8_t component = components[j];
            for (uint8_t k = 0; k < 8; k++) { // for each bit, starting from the most significant
                bool bit = (component >> (7 - k)) & 1;
                tab[++index] = false;
                tab[++index] = !bit;
                tab[++index] = true;
                tab[++index] = true;
            }
        }
    }

    set_rgb_leds_color(tab);
}