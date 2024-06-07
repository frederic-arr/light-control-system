#ifndef __TL_H__
#define __TL_H__

#include <stdint.h>
#include <stdbool.h>

typedef enum _traffic_light_state_t {
	WAIT,
	READY,
	GO,
	STOP,
	OFF,
} traffic_light_state_t;

typedef enum _intersection_state_t {
	CHAtoGVA_GVAtoCHA_GVAtoVEG,
	GVAtoVEG_VEGtoGVA,
	CHAtoGVA_CHAtoVEG,
} intersection_state_t;

typedef struct _traffic_light_t {
	traffic_light_state_t state;
	uint16_t x;
	uint16_t y;
} traffic_light_t;

typedef struct _intersection_t {
	uint16_t time;
	uint8_t stabilization_factor;
	bool is_disabled;
	bool is_blocked;
	intersection_state_t state;
	traffic_light_t cha_gva;
	traffic_light_t cha_veg;
	traffic_light_t gva_cha;
	traffic_light_t gva_veg;
	traffic_light_t veg_gva;
} intersection_t;

intersection_t inter_init(void);
void inter_transition(intersection_t* inter);
void inter_block(intersection_t* inter);
void inter_disable(intersection_t* inter);
void inter_enable(intersection_t* inter);
void inter_draw(intersection_t* inter, bool is_dark);
traffic_light_t tl_new(traffic_light_state_t state, uint16_t x, uint16_t y);
void tl_draw(traffic_light_t* tl, bool is_dark);
void tl_next(traffic_light_t* tl);
void tl_to_wait(traffic_light_t* tl);
void tl_to_go(traffic_light_t* tl);

#endif
