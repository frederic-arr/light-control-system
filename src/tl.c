#include "tl.h"

#include <stdbool.h>
#include <stdint.h>
#include <tlm.h>

#include "sprites/light_ped_go.h"
#include "sprites/light_ped_off.h"
#include "sprites/light_ped_stop.h"
#include "sprites/light_veh_go.h"
#include "sprites/light_veh_off.h"
#include "sprites/light_veh_ready.h"
#include "sprites/light_veh_stop.h"
#include "sprites/light_veh_wait.h"
#include "sprites/light_warn_off.h"
#include "sprites/light_warn_on.h"
#include "sprites/light_req_off.h"
#include "sprites/light_req_on.h"

uint16_t lights[5][2] = {
    {38, 70}, {68, 70}, {90, 216}, {136, 216}, {195, 135},
};
uint8_t* sprites[6] = {light_veh_wait, light_veh_ready, light_veh_go,
                       light_veh_stop, light_veh_off,   light_veh_stop};
uint8_t* sprites_pedestrian[6]
    = {light_ped_stop, light_ped_stop, light_ped_go,
       light_ped_stop, light_ped_off,  light_ped_stop};
uint8_t* sprites_warn[6] = {light_warn_on, light_warn_on,  light_warn_on,
                           light_warn_on, light_warn_off, light_warn_on};
uint8_t* sprites_req[2] = {light_req_off, light_req_on};

struct Intersection* intersection = NULL;

void tl_init() {
    tlm_init();
    intersection = tlm_intersection_new();
}

struct Intersection* tl_get() { return intersection; }

void tl_draw(bool is_dark) {
    enum Sprite* state = tlm_intersection_get_lights(intersection);
    for (int i = 0; i < 5; i++) {
        Sprite s = state[i];
        DRAW_SPRITE(lights[i][0], lights[i][1], 14, 32, sprites[s], is_dark);
    }

    DRAW_SPRITE(165, 150, 18, 16, sprites_pedestrian[state[5]], is_dark);
    DRAW_SPRITE(85, 110, 18, 16, sprites_pedestrian[state[6]], is_dark);
    DRAW_SPRITE(200, 65, 14, 14, sprites_warn[state[7]], is_dark);
    DRAW_SPRITE(196, 206, 12, 12, sprites_req[tlm_intersection_has_pedestrian_request(intersection)], is_dark);
    DRAW_SPRITE(170, 109, 12, 12, sprites_req[tlm_intersection_has_pedestrian_request(intersection)], is_dark);
}
