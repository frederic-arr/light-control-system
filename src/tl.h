#ifndef __TL_H__
#define __TL_H__

#include <tlm.h>

void tl_init();
struct Intersection* tl_get();
void tl_draw(bool is_dark);

#endif
