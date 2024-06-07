#include "tl.h"
#include "sprites/light_veh_off.h"
#include "sprites/light_veh_wait.h"
#include "sprites/light_veh_ready.h"
#include "sprites/light_veh_go.h"
#include "sprites/light_veh_stop.h"

intersection_t inter_init(void) {
	traffic_light_t cha_gva = tl_new(GO, 38, 70);
	traffic_light_t cha_veg = tl_new(WAIT, 68, 70);
	traffic_light_t gva_cha = tl_new(GO, 90, 216);
	traffic_light_t gva_veg = tl_new(GO, 136, 216);
	traffic_light_t veg_gva = tl_new(WAIT, 195, 135);
    return (intersection_t) {
        .stabilization_factor = 0,
        .state = CHAtoGVA_GVAtoCHA_GVAtoVEG,
        .cha_gva = cha_gva,
        .cha_veg = cha_veg,
        .gva_cha = gva_cha,
        .gva_veg = gva_veg,
        .veg_gva = veg_gva,
    };
}

void inter_disable(intersection_t* inter) {
	inter->is_disabled = true;
	tl_to_wait(&inter->cha_gva);
	tl_to_wait(&inter->cha_veg);
	tl_to_wait(&inter->gva_cha);
	tl_to_wait(&inter->gva_veg);
	tl_to_wait(&inter->veg_gva);
}

void inter_block(intersection_t* inter) {
	inter->is_blocked = true;
	inter->time = 0;
	inter->stabilization_factor = 0;
	tl_to_wait(&inter->cha_gva);
	tl_to_wait(&inter->cha_veg);
	tl_to_wait(&inter->gva_cha);
	tl_to_wait(&inter->gva_veg);
	tl_to_wait(&inter->veg_gva);
}

void inter_enable(intersection_t* inter) {
	inter->is_disabled = false;
	tl_to_wait(&inter->cha_gva);
	tl_to_wait(&inter->cha_veg);
	tl_to_wait(&inter->gva_cha);
	tl_to_wait(&inter->gva_veg);
	tl_to_wait(&inter->veg_gva);
}

void inter_transition(intersection_t* inter) {
	if (inter->is_blocked) {
		tl_to_wait(&inter->cha_gva);
		tl_to_wait(&inter->cha_veg);
		tl_to_wait(&inter->gva_cha);
		tl_to_wait(&inter->gva_veg);
		tl_to_wait(&inter->veg_gva);
		if (inter->time >= 200) {
			inter->is_blocked = false;
		}
		return;
	}

	uint16_t reset = inter->is_disabled ? 25 : 150;
	if (inter->time % reset == 0) {
		inter->stabilization_factor = (inter->stabilization_factor + 1) % 2;
		inter->time = 0;
	} else {
		return;
	}

	if (inter->is_disabled) {
		bool blink = inter->stabilization_factor == 0;
		inter->cha_gva.state = blink ? OFF : STOP;
		inter->cha_veg.state = blink ? OFF : STOP;
		inter->gva_cha.state = blink ? OFF : STOP;
		inter->gva_veg.state = blink ? OFF : STOP;
		inter->veg_gva.state = blink ? OFF : STOP;
		return;
	}

	switch (inter->state) {
		case CHAtoGVA_GVAtoCHA_GVAtoVEG:
			tl_to_go(&inter->cha_gva);
			tl_to_wait(&inter->cha_veg);
			tl_to_go(&inter->gva_cha);
			tl_to_go(&inter->gva_veg);
			tl_to_wait(&inter->veg_gva);
			if (inter->stabilization_factor == 0) {
				inter->state = GVAtoVEG_VEGtoGVA;
			}
			break;
		case GVAtoVEG_VEGtoGVA:
			tl_to_wait(&inter->cha_gva);
			tl_to_wait(&inter->cha_veg);
			tl_to_wait(&inter->gva_cha);
			tl_to_go(&inter->gva_veg);
			tl_to_go(&inter->veg_gva);
			if (inter->stabilization_factor == 0) {
				inter->state = CHAtoGVA_CHAtoVEG;
			}
			break;
		case CHAtoGVA_CHAtoVEG:
			tl_to_go(&inter->cha_gva);
			tl_to_go(&inter->cha_veg);
			tl_to_wait(&inter->gva_cha);
			tl_to_wait(&inter->gva_veg);
			tl_to_wait(&inter->veg_gva);
			if (inter->stabilization_factor == 0) {
				inter->state = CHAtoGVA_GVAtoCHA_GVAtoVEG;
			}
			break;
	}
}

void inter_draw(intersection_t* inter, bool is_dark) {
	tl_draw(&inter->cha_gva, is_dark);
	tl_draw(&inter->cha_veg, is_dark);
	tl_draw(&inter->gva_cha, is_dark);
	tl_draw(&inter->gva_veg, is_dark);
	tl_draw(&inter->veg_gva, is_dark);
}

traffic_light_t tl_new(traffic_light_state_t state, uint16_t x, uint16_t y) {
	return (traffic_light_t) {
		.state = state,
		.x = x,
		.y = y,
	};
}

uint8_t* lights[5] = {light_veh_wait, light_veh_ready, light_veh_go, light_veh_stop, light_veh_off};
void tl_draw(traffic_light_t* tl, bool is_dark) {
	DRAW_SPRITE(tl->x, tl->y, 14, 32, lights[tl->state], is_dark);
}

void tl_next(traffic_light_t* tl) {
	tl->state = (tl->state + 1) % 4;
}

void tl_to_wait(traffic_light_t* tl) {
	switch (tl->state) {
		case READY:
			tl->state = WAIT;
			break;
		case GO:
			tl->state = STOP;
			break;
		case STOP:
			tl->state = WAIT;
			break;
	}
}

void tl_to_go(traffic_light_t* tl) {
	switch (tl->state) {
		case WAIT:
			tl->state = READY;
			break;
		case READY:
			tl->state = GO;
			break;
		case STOP:
			tl->state = GO;
			break;
	}
}
