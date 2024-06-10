#ifndef __LUMINOSITY_H__
#define __LUMINOSITY_H__

// https://hepialsn.hesge.ch/myLab2/datasheets/APDS-9960.pdf

#include <stdbool.h>

void LUMINOSITY_INIT(void);
bool LUMINOSITY_IS_DARK(void);

#endif
