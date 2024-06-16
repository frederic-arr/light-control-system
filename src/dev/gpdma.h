#ifndef __GPDMA_H__
#define __GPDMA_H__

#include <stdbool.h>
#include <stdint.h>

#define DMACIntStat (*(volatile uint32_t *)0x50004000)
#define DMACIntTCStat (*(volatile uint32_t *)0x50004004)
#define DMACIntTCClear (*(volatile uint32_t *)0x50004008)
#define DMACIntErrStat (*(volatile uint32_t *)0x5000400C)
#define DMACIntErrClr (*(volatile uint32_t *)0x50004010)
#define DMACRawIntTCStat (*(volatile uint32_t *)0x50004014)
#define DMACRawIntErrStat (*(volatile uint32_t *)0x50004018)
#define DMACEnbldChns (*(volatile uint32_t *)0x5000401C)
#define DMACSoftBReq (*(volatile uint32_t *)0x50004020)
#define DMACSoftSReq (*(volatile uint32_t *)0x50004024)
#define DMACSoftLBReq (*(volatile uint32_t *)0x50004028)
#define DMACSoftLSReq (*(volatile uint32_t *)0x5000402C)
#define DMACConfig (*(volatile uint32_t *)0x50004030)
#define DMACSync (*(volatile uint32_t *)0x50004034)
#define DMAREQSEL (*(volatile uint32_t *)0x400FC1C4)
#define DMACC0SrcAddr (*(volatile uint32_t *)0x50004100)
#define DMACC0DestAddr (*(volatile uint32_t *)0x50004104)
#define DMACC0LLI (*(volatile uint32_t *)0x50004108)
#define DMACC0Control (*(volatile uint32_t *)0x5000410C)
#define DMACC0Config (*(volatile uint32_t *)0x50004110)

#define _GPDMA_CONFIG_MASK 0b11
#define _GPDMA_CHANNEL_CONFIG_MASK 0x3FFFF
#define _GPDMA_TRANSFER_CONFIG_MASK 0x1FFFFFFF

typedef enum gpdma_channel_t {
  GPDMA_CHANNEL_0 = 0,
  GPDMA_CHANNEL_1 = 1,
  GPDMA_CHANNEL_2 = 2,
  GPDMA_CHANNEL_3 = 3,
  GPDMA_CHANNEL_4 = 4,
  GPDMA_CHANNEL_5 = 5,
  GPDMA_CHANNEL_6 = 6,
  GPDMA_CHANNEL_7 = 7,
} gpdma_channel_t;

typedef struct _gpdma_configuration_t {
  bool enable;
  bool is_big_endian;
} gpdma_configuration_t;

typedef enum _gpdma_peripheral_t {
  GPDMA_PERIPHERAL_SSP0_TX = 0,
  GPDMA_PERIPHERAL_SSP0_RX = 1,
  GPDMA_PERIPHERAL_SSP1_TX = 2,
  GPDMA_PERIPHERAL_SSP1_RX = 3,
  GPDMA_PERIPHERAL_ADC = 4,
  GPDMA_PERIPHERAL_I2S_C0 = 5,
  GPDMA_PERIPHERAL_I2S_C1 = 6,
  GPDMA_PERIPHERAL_DAC = 7,

  GPDMA_PERIPHERAL_UART0_TX = 8,
  GPDMA_PERIPHERAL_UART0_RX = 9,
  GPDMA_PERIPHERAL_UART1_TX = 10,
  GPDMA_PERIPHERAL_UART1_RX = 11,
  GPDMA_PERIPHERAL_UART2_TX = 12,
  GPDMA_PERIPHERAL_UART2_RX = 13,
  GPDMA_PERIPHERAL_UART3_TX = 14,
  GPDMA_PERIPHERAL_UART3_RX = 15,

  GPDMA_PERIPHERAL_MAT0_0 = 8,
  GPDMA_PERIPHERAL_MAT0_1 = 9,
  GPDMA_PERIPHERAL_MAT1_0 = 10,
  GPDMA_PERIPHERAL_MAT1_1 = 11,
  GPDMA_PERIPHERAL_MAT2_0 = 12,
  GPDMA_PERIPHERAL_MAT2_1 = 13,
  GPDMA_PERIPHERAL_MAT3_0 = 14,
  GPDMA_PERIPHERAL_MAT3_1 = 15,
} gpdma_peripheral_t;

typedef enum _gpdma_transfer_type_t {
  GPDMA_TRANSFER_TYPE_M2M = 0b000,
  GPDMA_TRANSFER_TYPE_M2P = 0b001,
  GPDMA_TRANSFER_TYPE_P2M = 0b010,
  GPDMA_TRANSFER_TYPE_P2P = 0b011,
} gpdma_transfer_type_t;

typedef struct _gpdma_channel_configuration_t {
  bool enable;
  gpdma_peripheral_t src_peripheral;
  gpdma_peripheral_t dest_peripheral;
  gpdma_transfer_type_t transfer_type;
  bool enable_error_interrupt;
  bool enable_terminal_count_interrupt;

  bool halt;
} gpdma_channel_configuration_t;

typedef enum _gpdma_channel_control_width_t {
  GPDMA_CHANNEL_CONTROL_WIDTH_BYTE = 0b000,
  GPDMA_CHANNEL_CONTROL_WIDTH_HALF = 0b001,
  GPDMA_CHANNEL_CONTROL_WIDTH_WORD = 0b010,
} gpdma_channel_control_width_t;

typedef struct _gpdma_transfert_configuration_t {
  uint16_t transfer_size : 12;
  uint8_t source_burst_size : 3;
  uint8_t destination_burst_size : 3;
  gpdma_channel_control_width_t source_width : 3;
  gpdma_channel_control_width_t destination_width : 3;
  uint8_t _reserved : 2;
  bool enable_source_increment : 1;
  bool enable_destination_increment : 1;

  bool enable_terminal_count_interrupt : 1;
} gpdma_transfert_configuration_t;

void gpdma_configure(gpdma_configuration_t config);
void gpdma_configure_channel(gpdma_channel_t channel,
                             gpdma_channel_configuration_t config);
void gpdma_configure_transfer(gpdma_channel_t channel, uint32_t *src,
                              uint32_t *dst,
                              gpdma_transfert_configuration_t config);

#endif  // __GPDMA_H__
