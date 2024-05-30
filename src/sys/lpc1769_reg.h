/*
 * lpc1769_reg.h
 *
 *  Created on: 26 Apr 2020
 *      Author: fabien
 */

#ifndef LPC1769_REG_H_
#define LPC1769_REG_H_

#define	FIO0DIR			(*((volatile uint32_t *) 0x2009C000))
#define	FIO0MASK		(*((volatile uint32_t *) 0x2009C010))
#define	FIO0PIN			(*((volatile uint32_t *) 0x2009C014))
#define	FIO0SET			(*((volatile uint32_t *) 0x2009C018))
#define	FIO0CLR			(*((volatile uint32_t *) 0x2009C01C))

#define	FIO1DIR			(*((volatile uint32_t *) 0x2009C020))
#define	FIO1MASK		(*((volatile uint32_t *) 0x2009C030))
#define	FIO1PIN			(*((volatile uint32_t *) 0x2009C034))
#define	FIO1SET			(*((volatile uint32_t *) 0x2009C038))
#define	FIO1CLR			(*((volatile uint32_t *) 0x2009C03C))

#define	FIO2DIR			(*((volatile uint32_t *) 0x2009C040))
#define	FIO2MASK		(*((volatile uint32_t *) 0x2009C050))
#define	FIO2PIN			(*((volatile uint32_t *) 0x2009C054))
#define	FIO2SET			(*((volatile uint32_t *) 0x2009C058))
#define	FIO2CLR			(*((volatile uint32_t *) 0x2009C05C))

#define	FIO3DIR			(*((volatile uint32_t *) 0x2009C060))
#define	FIO3MASK		(*((volatile uint32_t *) 0x2009C070))
#define	FIO3PIN			(*((volatile uint32_t *) 0x2009C074))
#define	FIO3SET			(*((volatile uint32_t *) 0x2009C078))
#define	FIO3CLR			(*((volatile uint32_t *) 0x2009C07C))

#define	FIO4DIR			(*((volatile uint32_t *) 0x2009C080))
#define	FIO4MASK		(*((volatile uint32_t *) 0x2009C090))
#define	FIO4PIN			(*((volatile uint32_t *) 0x2009C094))
#define	FIO4SET			(*((volatile uint32_t *) 0x2009C098))
#define	FIO4CLR			(*((volatile uint32_t *) 0x2009C09C))



#define	CCR				(*((volatile uint32_t *) 0xE000ED14))
#define	SysTick_CTRL	(*((volatile uint32_t *) 0xE000E010))
#define	SysTick_LOAD	(*((volatile uint32_t *) 0xE000E014))


#define PINSEL0			(*((volatile uint32_t *) 0x4002C000))
#define PINSEL1			(*((volatile uint32_t *) 0x4002C004))

#define S0SPCR			(*((volatile uint32_t *) 0x40020000))
#define S0SPSR			(*((volatile uint32_t *) 0x40020004))
#define S0SPDR			(*((volatile uint32_t *) 0x40020008))
#define S0SPCCR			(*((volatile uint32_t *) 0x4002000C))
#define S0SPINT			(*((volatile uint32_t *) 0x4002001C))

#define STCTRL			(*((volatile uint32_t *) 0xE000E010))
#define STRELOAD		(*((volatile uint32_t *) 0xE000E014))


// Syscon Miscellaneous Registers
#define SCS				(*((volatile uint32_t *) 0x400FC1A0))
// CLK SOURCE SELECTION
#define CLKSRCSEL		(*((volatile uint32_t *) 0x400FC10C))
// PLL0, Main PLL
#define PLL0CON			(*((volatile uint32_t *) 0x400FC080))
#define PLL0CFG			(*((volatile uint32_t *) 0x400FC084))
#define PLL0STAT		(*((volatile uint32_t *) 0x400FC088))
#define PLL0FEED		(*((volatile uint32_t *) 0x400FC08C))
// PLL1, UBS PLL
#define PLL1CON			(*((volatile uint32_t *) 0x400FC0A0))
#define PLL1CFG			(*((volatile uint32_t *) 0x400FC0A4))
#define PLL1STAT		(*((volatile uint32_t *) 0x400FC0A8))
#define PLL1FEED		(*((volatile uint32_t *) 0x400FC0AC))
// CLOCK DIVIDER
#define CCLKCFG			(*((volatile uint32_t *) 0x400FC104))
#define USBCLKCFG		(*((volatile uint32_t *) 0x400FC108))
#define PCLKSEL0		(*((volatile uint32_t *) 0x400FC1A8))
#define PCLKSEL1		(*((volatile uint32_t *) 0x400FC1AC))
// POWER CONTROL
#define PCON			(*((volatile uint32_t *) 0x400FC0C0))
#define PCONP			(*((volatile uint32_t *) 0x400FC0C4))
// Utility
#define CLKOUTCFG		(*((volatile uint32_t *) 0x400FC1C8))

// Flash Accelerator
#define FLASHCFG		(*((volatile uint32_t *) 0x400FC000))

#endif /* LPC1769_REG_H_ */
