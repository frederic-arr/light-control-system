#ifndef __GPIO_H__
#define __GPIO_H__

#define IO0IntEnR			(*((volatile uint32_t *) 0x40028090))
#define IO2IntEnR			(*((volatile uint32_t *) 0x400280B0))
#define IO0IntEnF			(*((volatile uint32_t *) 0x40028094))
#define IO2IntEnF			(*((volatile uint32_t *) 0x400280B4))
#define IO0IntStatR			(*((volatile uint32_t *) 0x40028084))
#define IO2IntStatR			(*((volatile uint32_t *) 0x400280A4))
#define IO0IntStatF			(*((volatile uint32_t *) 0x40028088))
#define IO2IntStatF			(*((volatile uint32_t *) 0x400280A8))
#define IO0IntClr			(*((volatile uint32_t *) 0x4002808C))
#define IO2IntClr			(*((volatile uint32_t *) 0x400280AC))
#define IOIntStatus			(*((volatile uint32_t *) 0x40028080))

void gpio_init(void);

#endif // __GPIO_H__
