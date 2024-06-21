#ifndef __APDS9960_H__
#define __APDS9960_H__

// https://hepialsn.hesge.ch/myLab2/datasheets/APDS-9960.pdf

#include <stdint.h>

void apds9960_init(void);
uint16_t apds9960_get_luminosity(void);

#endif
