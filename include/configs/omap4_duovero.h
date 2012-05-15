/*
 * Maintainer: Steve Sakoman  <steve@sakoman.com>
 *
 * Configuration settings for the Gumstix duovero board.
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

#include <configs/omap4_common.h>

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

#endif /* __CONFIG_DUOVERO_H */
