/*
 * am335x_pepper.h
 *
 * Copyright (C) 2013 Gumstix, Inc. - http://www.gumstix.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __CONFIG_AM335X_PEPPER_H
#define __CONFIG_AM335X_PEPPER_H

#include <configs/am335x_evm.h>
#undef CONFIG_EXTRA_ENV_SETTINGS
#undef CONFIG_MACH_TYPE
#undef CONFIG_BOOTDELAY
#undef CONFIG_BOOTCOMMAND
#undef CONFIG_SYS_PROMPT
#undef CONFIG_BOARD_LATE_INIT

#define CONFIG_CONS_INDEX 1

#define MACH_TYPE_PEPPER		4207	/* Until the next sync */
#define CONFIG_MACH_TYPE		MACH_TYPE_PEPPER
#define CONFIG_SYS_PROMPT		"pepper# "

#define CONFIG_SYS_MMC_SET_DEV

#undef CONFIG_PHY_SMSC
#define CONFIG_CMD_MII
#define CONFIG_PHY_MICREL
#define CONFIG_PHY_MICREL_KSZ9021
#define CONFIG_PHY_RESET_DELAY 1000

#define CONFIG_BOOTDELAY		3
#define CONFIG_EXTRA_ENV_SETTINGS \
	"loadaddr=0x80200000\0" \
	"fdtaddr=0x80F80000\0" \
	"fdt_high=0xffffffff\0" \
	"rdaddr=0x81000000\0" \
	"bootdir=/boot\0" \
	"bootfile=uImage\0" \
	"fdtfile=undefined\0" \
	"console=ttyO0,115200n8\0" \
	"optargs=\0" \
	"mmcdev=0\0" \
	"mmcroot=/dev/mmcblk0p2 rw\0" \
	"mmcrootfstype=ext3 rootwait\0" \
	"bootpart=0:2\0" \
	"ramroot=/dev/ram0 rw ramdisk_size=65536 initrd=${rdaddr},64M\0" \
	"ramrootfstype=ext2\0" \
	"mmcargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=${mmcroot} " \
		"rootfstype=${mmcrootfstype}\0" \
	"bootenv=uEnv.txt\0" \
	"loadbootenv=load mmc ${mmcdev} ${loadaddr} ${bootenv}\0" \
	"importbootenv=echo Importing environment from mmc ...; " \
		"env import -t $loadaddr $filesize\0" \
	"ramargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=${ramroot} " \
		"rootfstype=${ramrootfstype}\0" \
	"loadramdisk=load mmc ${mmcdev} ${rdaddr} ramdisk.gz\0" \
	"loaduimage=load mmc ${bootpart} ${loadaddr} ${bootdir}/${bootfile}\0" \
	"loadfdt=load mmc ${bootpart} ${fdtaddr} ${bootdir}/${fdtfile}\0" \
	"mmcboot=echo Booting from mmc ...; " \
		"run mmcargs; " \
		"bootm ${loadaddr}\0" \
	"ramboot=echo Booting from ramdisk ...; " \
		"run ramargs; " \
		"bootm ${loadaddr} ${rdaddr}\0" \

#define CONFIG_BOOTCOMMAND \
	"mmc dev ${mmcdev}; if mmc rescan; then " \
		"echo SD/MMC found on device ${mmcdev};" \
		"if run loadbootenv; then " \
			"echo Loaded environment from ${bootenv};" \
			"run importbootenv;" \
		"fi;" \
		"if test -n $uenvcmd; then " \
			"echo Running uenvcmd ...;" \
			"run uenvcmd;" \
		"fi;" \
		"if run loaduimage; then " \
			"run mmcboot;" \
		"fi;" \
	"fi;" \


#endif
