/*
 * (C) Copyright: 2013
 * Gumstix, Inc - http://www.gumstix.com
 * Maintainer: Ash Charles  <ash@gumstix.com>
 *
 * Configuration settings for the Gumstix DuoVero board.
 * See omap4_common.h for OMAP4 common part
 *
 * SPDX-License-Identifier:     GPL-2.0+
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
