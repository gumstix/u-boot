/*
 * (C) Copyright: 2013
 * Gumstix, Inc
 * Maintainer: Ash Charles  <ash@gumstix.com>
 *
 * Configuration settings for the Gumstix DuoVero board.
 * See omap4_common.h for OMAP4 common part
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_DUOVERO_H
#define __CONFIG_DUOVERO_H

/*
 * High Level Configuration Options
 */
#define CONFIG_DUOVERO		1
#define MACH_TYPE_OMAP4_DUOVERO              4097    /* Until the next sync */
#define CONFIG_MACH_TYPE                MACH_TYPE_OMAP4_DUOVERO

#include <configs/omap4_common.h>

#undef CONFIG_SYS_EMIF_PRECALCULATED_TIMING_REGS
#define CONFIG_SYS_AUTOMATIC_SDRAM_DETECTION
#define CONFIG_SYS_DEFAULT_LPDDR2_TIMINGS

#define CONFIG_SYS_ENABLE_PADS_ALL
 
#define V_OSCK			38400000	/* Clock output from T2 */
#define V_SCLK                   V_OSCK

#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_SYS_PROMPT		"duovero # "

#define CONFIG_CMD_NET		/* bootp, tftpboot, rarpboot	*/
#define CONFIG_CMD_DHCP

#if defined(CONFIG_CMD_NET)
#define CONFIG_SMC911X
#define CONFIG_SMC911X_32_BIT
#define CONFIG_SMC911X_BASE		0x2C000000
#endif /* (CONFIG_CMD_NET) */

/* USB UHH support options */
#define CONFIG_CMD_USB
#define CONFIG_USB_HOST
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_OMAP
#define CONFIG_USB_STORAGE
#define CONFIG_SYS_USB_EHCI_MAX_ROOT_PORTS 3

#define CONFIG_OMAP_EHCI_PHY1_RESET_GPIO 1
#define CONFIG_OMAP_EHCI_PHY2_RESET_GPIO 62

#define CONFIG_USB_ULPI
#define CONFIG_USB_ULPI_VIEWPORT_OMAP

#endif /* __CONFIG_DUOVERO_H */
