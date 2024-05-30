/*
 * ledrgb.h
 *
 *  Created on: Mar 26, 2014
 *      Author: Adrien, V. Pilloux
 *   Adapted for PMC : FV (0520)
 */

#ifndef LEDRGB_H_
#define LEDRGB_H_

#include <stdint.h>
#include <stdbool.h>

/*
 * Setup the RGB led controller
 */
void init_rgb(void);

/*
 * Setup the RGB led with the desired RGB color
 * Parameters: Bool table with 4 elements for each bit color (see course documentation). Don't forget initial state
 * ATTENTION : BIT VALUE SHOULD BE INVERTED (Due to inverter on Mylab2 Board)
 */
void set_rgb_leds_color(bool * tab);

#endif /* LEDRGB_H_ */
