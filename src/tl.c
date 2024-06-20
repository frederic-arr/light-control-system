#include "tl.h"

#include <stdbool.h>
#include <stdint.h>
#include <tlm.h>

#include "sprites/light_veh_go.h"
#include "sprites/light_veh_off.h"
#include "sprites/light_veh_ready.h"
#include "sprites/light_veh_stop.h"
#include "sprites/light_veh_wait.h"

uint16_t lights[5][2] = {
    {38, 70}, {68, 70}, {90, 216}, {136, 216}, {195, 135},
};
uint8_t* sprites[5] = {light_veh_wait, light_veh_ready, light_veh_go,
                       light_veh_stop, light_veh_off};
void tl_draw() {
    enum TrafficLight* state = tlm_intersection_get_lights();
    for (int i = 0; i < 5; i++) {
        DRAW_SPRITE(lights[i][0], lights[i][1], 14, 32, sprites[state[i]],
                    false);
    }
}
