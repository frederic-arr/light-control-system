#ifndef __LUMINOSITY_H__
#define __LUMINOSITY_H__

// https://hepialsn.hesge.ch/myLab2/datasheets/APDS-9960.pdf

#include <stdbool.h>

void apds9960_init(void);
uint16_t apds9960_get_luminosity(void);

#endif
