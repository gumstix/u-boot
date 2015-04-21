/*
 * Maintainer : Steve Sakoman <steve@sakoman.com>
 *
 * Derived from Beagle Board, 3430 SDP, and OMAP3EVM code by
 *      Richard Woodruff <r-woodruff2@ti.com>
 *      Syed Mohammed Khasim <khasim@ti.com>
 *      Sunil Kumar <sunilsaini05@gmail.com>
 *      Shashi Ranjan <shashiranjanmca05@gmail.com>
 *
 * (C) Copyright 2004-2008
 * Texas Instruments, <www.ti.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#include <twl4030.h>
#include <asm/io.h>
#include <asm/arch/mem.h>
#include <asm/arch/sys_proto.h>
#include <asm/gpio.h>
#include "overo.h"

#define TWL4030_I2C_BUS                 0

/*
 * Routine: get_board_revision
 * Description: Returns the board revision
 */
int get_board_revision(void)
{
        int revision;

        if (!gpio_request(112, "") &&
            !gpio_request(113, "") &&
            !gpio_request(115, "")) {

                gpio_direction_input(112);
                gpio_direction_input(113);
                gpio_direction_input(115);

                revision = gpio_get_value(115) << 2 |
                           gpio_get_value(113) << 1 |
                           gpio_get_value(112);
        } else {
                puts("Error: unable to acquire board revision GPIOs\n");
                revision = -1;
        }

	printf("Board revision: %d\n", revision);
        return revision;
}

/*
 * Routine: get_board_mem_timings
 * Description: If we use SPL then there is no x-loader nor config header
 * so we have to setup the DDR timings ourself on both banks.
 */
void get_board_mem_timings(struct board_sdrc_timings *timings)
{
        timings->mr = MICRON_V_MR_165;
        switch (get_board_revision()) {
        case REVISION_0: /* Micron 1286MB/256MB, 1/2 banks of 128MB */
                timings->mcfg = MICRON_V_MCFG_165(128 << 20);
                timings->ctrla = MICRON_V_ACTIMA_165;
                timings->ctrlb = MICRON_V_ACTIMB_165;
                timings->rfr_ctrl = SDP_3430_SDRC_RFR_CTRL_165MHz;
                break;
        case REVISION_1: /* Micron 256MB/512MB, 1/2 banks of 256MB */
        case REVISION_4:
                timings->mcfg = MICRON_V_MCFG_200(256 << 20);
                timings->ctrla = MICRON_V_ACTIMA_200;
                timings->ctrlb = MICRON_V_ACTIMB_200;
                timings->rfr_ctrl = SDP_3430_SDRC_RFR_CTRL_200MHz;
                break;
        case REVISION_2: /* Hynix 256MB/512MB, 1/2 banks of 256MB */
                timings->mcfg = HYNIX_V_MCFG_200(256 << 20);
                timings->ctrla = HYNIX_V_ACTIMA_200;
                timings->ctrlb = HYNIX_V_ACTIMB_200;
                timings->rfr_ctrl = SDP_3430_SDRC_RFR_CTRL_200MHz;
                break;
        case REVISION_3: /* Micron 512MB/1024MB, 1/2 banks of 512MB */
                timings->mcfg = MCFG(512 << 20, 15);
                timings->ctrla = MICRON_V_ACTIMA_200;
                timings->ctrlb = MICRON_V_ACTIMB_200;
                timings->rfr_ctrl = SDP_3430_SDRC_RFR_CTRL_200MHz;
                break;
        default:
                timings->mcfg = MICRON_V_MCFG_165(128 << 20);
                timings->ctrla = MICRON_V_ACTIMA_165;
                timings->ctrlb = MICRON_V_ACTIMB_165;
                timings->rfr_ctrl = SDP_3430_SDRC_RFR_CTRL_165MHz;
        }
}


