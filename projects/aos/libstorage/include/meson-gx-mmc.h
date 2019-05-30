/*
 * Hardware definition of the meson-gx-mmc device.
 */

#include <utils/arith.h>
#include <stdint.h>

#pragma once

/*
 * An instance of a meson device.
 */
typedef struct meson_gx_mmc {
    uint32_t *registers;
} meson_gx_mmc_t;

#define MESON_SD_EMMC_CLOCK              0x00
#define   MESON_CLOCK_DIV_MAX            63
#define   MESON_CLOCK_DIVIDER            BIT(0)
#define   MESON_CLOCK_DIVIDER_MASK       MASK(6)
#define   MESON_CLOCK_SOURCE             BIT(6)
#define   MESON_CLOCK_SOURCE_MASK        MASK(2)
#define   MESON_CLOCK_CO_PHASE           BIT(8)
#define   MESON_CLOCK_CO_PHASE_MASK      MASK(2)
#define   MESON_CLOCK_TX_PHASE           BIT(10)
#define   MESON_CLOCK_TX_PHASE_MASK      MASK(2)
#define   MESON_CLOCK_RX_PHASE           BIT(12)
#define   MESON_CLOCK_RX_PHASE_MASK      MASK(2)
#define   MESON_CLOCK_SRAM_PD            BIT(14)
#define   MESON_CLOCK_SRAM_PD_MASK       MASK(2)
#define   MESON_CLOCK_CFG_IRQ_SDIO_SLEEP BIT(25)
#define

#define MESON_SD_EMMC_DELAY    0x04
#define MESON_SD_EMMC_ADJUST   0x08
#define MESON_SD_EMMC_CALOUT   0x10
#define MESON_SD_EMMC_START    0x40
#define MESON_SD_EMMC_CFG      0x44
#define MESON_EMMC_STATUS      0x48
#define MESON_EMMC_IRQ_EN      0x4c

/*
 * The descriptor is a single command that is executed by the eMMC
 * device.
 */

typedef struct descriptor {
    uint32_t words[4];
} descriptor_t;

#define MESON_DESC_CFG              0x00
#define   MESON_DESC_LENGTH         BIT(0)
#define   MESON_DESC_LENGTH_MASK    MASK(9)
#define   MESON_DESC_BLOCK_MODE     BIT(9)
#define   MESON_DESC_R1B            BIT(10)
#define   MESON_DESC_END_CHAIN      BIT(11)
#define   MESON_DESC_TOUT           BIT(12)
#define   MESON_DESC_TOUT_MASK      MASK(4)
#define   MESON_DESC_NO_RESP        BIT(16)
#define   MESON_DESC_NO_CMD         BIT(17)
#define   MESON_DESC_DATA_IO        BIT(18)
#define   MESON_DESC_DATA_WR        BIT(19)
#define   MESON_DESC_RESP_NO_CRC    BIT(20)
#define   MESON_DESC_RESP_128       BIT(21)
#define   MESON_DESC_RESP_NUM       BIT(22)
#define   MESON_DESC_DATA_NUM       BIT(23)
#define   MESON_DESC_CMD_INDEX      BIT(24)
#define   MESON_DESC_CMD_INDEX_MASK MASK(6)
#define   MESON_DESC_ERROR          BIT(30)
#define   MESON_DESC_OWNER          BIT(31)

#define MESON_DESC_ARG              0x01

#define MESON_DESC_DATA             0x02
#define   MESON_DATA_SRAM           BIT(0)
#define   MESON_DATA_BE             BIT(1)
#define   MESON_DATA_ADDRESS        BIT(0)
#define   MESON_DATA_ADDRESS_MASK   (~MASK(2))

#define MESON_DESC_RESP             0x03
#define   MESON_RESP_SRAM           BIT(0)
#define   MESON_RESP_ADDRESS        BIT(0)
#define   MESON_RESO_ADDRESS_MASK   (~MASK(1))
