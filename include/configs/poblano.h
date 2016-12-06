/*
 * am43xx_evm.h
 *
 * Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_POBLANO_H
#define __CONFIG_POBLANO_H

#define CONFIG_AM43XX

#define CONFIG_CMD_FAT

#define CONFIG_BOARD_LATE_INIT
#define CONFIG_ARCH_CPU_INIT
#define CONFIG_SYS_CACHELINE_SIZE       32
#define CONFIG_MAX_RAM_BANK_SIZE	(1024 << 21)	/* 2GB */
#define CONFIG_SYS_TIMERBASE		0x48040000	/* Use Timer2 */

#include <asm/arch/omap.h>

/* NS16550 Configuration */
#define CONFIG_SYS_NS16550_CLK		48000000
#if defined(CONFIG_SPL_BUILD) || !defined(CONFIG_DM_SERIAL)
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	(-4)
#endif

/* I2C Configuration */
#define CONFIG_CMD_EEPROM
#define CONFIG_ENV_EEPROM_IS_ON_I2C
#define CONFIG_SYS_I2C_EEPROM_ADDR	0x50	/* Main EEPROM */
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN	2

/* Power */
#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_TPS65218
#define CONFIG_POWER_TPS62362

/* SPL defines. */
#ifdef CONFIG_SPL_USB_HOST_SUPPORT
/*
 * For USB host boot, ROM uses DMA for copying MLO from USB storage
 * and ARM internal ram is not accessible for DMA, so SPL text base
 * should be in OCMC ram
 */
#define CONFIG_SPL_TEXT_BASE		0x40300350
#else
#define CONFIG_SPL_TEXT_BASE		0x402F4000
#endif
#define CONFIG_SPL_MAX_SIZE		(220 << 10)	/* 220KB */
#define CONFIG_SYS_SPL_ARGS_ADDR	(CONFIG_SYS_SDRAM_BASE + \
					 (128 << 20))
#define CONFIG_SPL_POWER_SUPPORT
#define CONFIG_SPL_YMODEM_SUPPORT

/* Enabling L2 Cache */
#define CONFIG_SYS_L2_PL310
#define CONFIG_SYS_PL310_BASE	0x48242000
#define CONFIG_SYS_CACHELINE_SIZE	32

/*
 * Since SPL did pll and ddr initialization for us,
 * we don't need to do it twice.
 */
#if !defined(CONFIG_SPL_BUILD) && !defined(CONFIG_QSPI_BOOT)
#define CONFIG_SKIP_LOWLEVEL_INIT
#endif

/*
 * When building U-Boot such that there is no previous loader
 * we need to call board_early_init_f.  This is taken care of in
 * s_init when we have SPL used.
 */
#if !defined(CONFIG_SKIP_LOWLEVEL_INIT) && !defined(CONFIG_SPL)
#define CONFIG_BOARD_EARLY_INIT_F
#endif

/* Now bring in the rest of the common code. */
#include <configs/ti_armv7_omap.h>

/* Always 64 KiB env size */
#define CONFIG_ENV_SIZE			(64 << 10)

#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

/* Clock Defines */
#define V_OSCK				24000000  /* Clock output from T2 */
#define V_SCLK				(V_OSCK)

/* NS16550 Configuration */
#define CONFIG_SYS_NS16550_COM1		0x44e09000	/* Base EVM has UART0 */

#define CONFIG_ENV_IS_IN_FAT
#define FAT_ENV_INTERFACE		"mmc"
#define FAT_ENV_DEVICE_AND_PART		"0:1"
#define FAT_ENV_FILE			"uboot.env"
#define CONFIG_FAT_WRITE

#define CONFIG_SPL_LDSCRIPT		"$(CPUDIR)/omap-common/u-boot-spl.lds"

/* SPL USB Support */
#ifdef CONFIG_SPL_USB_HOST_SUPPORT
#define CONFIG_SPL_USB_SUPPORT
#define CONFIG_SYS_USB_FAT_BOOT_PARTITION		1

#define CONFIG_CMD_USB
#define CONFIG_USB_HOST
#define CONFIG_USB_XHCI
#define CONFIG_USB_XHCI_DWC3
#define CONFIG_USB_XHCI_OMAP
#define CONFIG_USB_STORAGE
#define CONFIG_SYS_USB_XHCI_MAX_ROOT_PORTS 2

#define CONFIG_OMAP_USB_PHY
#define CONFIG_AM437X_USB2PHY2_HOST
#endif

/* USB GADGET */
#if !defined(CONFIG_SPL_BUILD) || \
	(defined(CONFIG_SPL_BUILD) && defined(CONFIG_SPL_USBETH_SUPPORT))
#define CONFIG_USB_DWC3_PHY_OMAP
#define CONFIG_USB_DWC3_OMAP
#define CONFIG_USB_DWC3
#define CONFIG_USB_DWC3_GADGET

#define CONFIG_USB_GADGET
#define CONFIG_USB_GADGET_DOWNLOAD
#define CONFIG_USB_GADGET_VBUS_DRAW 2
#define CONFIG_G_DNL_MANUFACTURER "Texas Instruments"
#define CONFIG_G_DNL_VENDOR_NUM 0x0403
#define CONFIG_G_DNL_PRODUCT_NUM 0xBD00
#define CONFIG_USB_GADGET_DUALSPEED
#endif

/*
 * Disable MMC DM for SPL build and can be re-enabled after adding
 * DM support in SPL
 */
#ifdef CONFIG_SPL_BUILD
#undef CONFIG_DM_MMC
#endif

#ifndef CONFIG_SPL_BUILD
/* USB Device Firmware Update support */
#define CONFIG_USB_FUNCTION_DFU
#define CONFIG_DFU_RAM
#define CONFIG_CMD_DFU

#define CONFIG_DFU_MMC
#define DFU_ALT_INFO_MMC \
	"dfu_alt_info_mmc=" \
	"boot part 0 1;" \
	"rootfs part 0 2;" \
	"MLO fat 0 1;" \
	"spl-os-args fat 0 1;" \
	"spl-os-image fat 0 1;" \
	"u-boot.img fat 0 1;" \
	"uEnv.txt fat 0 1\0"

#define DFU_ALT_INFO_EMMC \
	"dfu_alt_info_emmc=" \
	"MLO raw 0x100 0x100 mmcpart 0;" \
	"u-boot.img raw 0x300 0x1000 mmcpart 0\0"

#define CONFIG_DFU_RAM
#define DFU_ALT_INFO_RAM \
	"dfu_alt_info_ram=" \
	"kernel ram 0x80200000 0x4000000;" \
	"fdt ram 0x80f80000 0x80000;" \
	"ramdisk ram 0x81000000 0x4000000\0"

#define CONFIG_DFU_SF
#define DFU_ALT_INFO_QSPI \
	"dfu_alt_info_qspi=" \
	"u-boot.bin raw 0x0 0x080000;" \
	"u-boot.backup raw 0x080000 0x080000;" \
	"u-boot-spl-os raw 0x100000 0x010000;" \
	"u-boot-env raw 0x110000 0x010000;" \
	"u-boot-env.backup raw 0x120000 0x010000;" \
	"kernel raw 0x130000 0x800000\0"

#define DFUARGS \
	"dfu_bufsiz=0x10000\0" \
	DFU_ALT_INFO_MMC \
	DFU_ALT_INFO_EMMC \
	DFU_ALT_INFO_RAM \
	DFU_ALT_INFO_QSPI
#else
#define DFUARGS
#endif

#ifdef CONFIG_QSPI_BOOT
#define CONFIG_SYS_TEXT_BASE           0x30000000
#undef CONFIG_ENV_IS_IN_FAT
#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT
#define CONFIG_ENV_SPI_MAX_HZ           CONFIG_SF_DEFAULT_SPEED
#define CONFIG_ENV_SECT_SIZE           (64 << 10) /* 64 KB sectors */
#define CONFIG_ENV_OFFSET              0x110000
#define CONFIG_ENV_OFFSET_REDUND       0x120000
#ifdef MTDIDS_DEFAULT
#undef MTDIDS_DEFAULT
#endif
#ifdef MTDPARTS_DEFAULT
#undef MTDPARTS_DEFAULT
#endif
#define MTDPARTS_DEFAULT		"mtdparts=qspi.0:512k(QSPI.u-boot)," \
					"512k(QSPI.u-boot.backup)," \
					"512k(QSPI.u-boot-spl-os)," \
					"64k(QSPI.u-boot-env)," \
					"64k(QSPI.u-boot-env.backup)," \
					"8m(QSPI.kernel)," \
					"-(QSPI.file-system)"
#endif

/* SPI */
#undef CONFIG_OMAP3_SPI
#define CONFIG_CMD_SF
#define CONFIG_CMD_SPI
#define CONFIG_TI_SPI_MMAP
#define CONFIG_QSPI_SEL_GPIO                   48
#define CONFIG_SF_DEFAULT_SPEED                48000000
#define CONFIG_DEFAULT_SPI_MODE                SPI_MODE_3
#define CONFIG_QSPI_QUAD_SUPPORT
#define CONFIG_TI_EDMA3

/* Enhance our eMMC support / experience. */
#define CONFIG_CMD_GPT
#define CONFIG_EFI_PARTITION

#ifndef CONFIG_SPL_BUILD
#define CONFIG_EXTRA_ENV_SETTINGS \
	DEFAULT_LINUX_BOOT_ENV \
	DEFAULT_MMC_TI_ARGS \
	"fdtfile=undefined\0" \
	"bootpart=0:2\0" \
	"bootdir=/boot\0" \
	"bootfile=zImage\0" \
	"console=ttyO0,115200n8\0" \
	"partitions=" \
		"uuid_disk=${uuid_gpt_disk};" \
		"name=rootfs,start=2MiB,size=-,uuid=${uuid_gpt_rootfs}\0" \
	"optargs=\0" \
	"usbroot=/dev/sda2 rw\0" \
	"usbrootfstype=ext4 rootwait\0" \
	"usbdev=0\0" \
	"ramroot=/dev/ram0 rw\0" \
	"ramrootfstype=ext2\0" \
	"usbargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=${usbroot} " \
		"rootfstype=${usbrootfstype}\0" \
	"bootenv=uEnv.txt\0" \
	"loadbootenv=load ${devtype} ${devnum} ${loadaddr} ${bootenv}\0" \
	"importbootenv=echo Importing environment from mmc ...; " \
		"env import -t $loadaddr $filesize\0" \
	"ramargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=${ramroot} " \
		"rootfstype=${ramrootfstype}\0" \
	"loadramdisk=load ${devtype} ${devnum} ${rdaddr} ramdisk.gz\0" \
	"loadimage=load ${devtype} ${bootpart} ${loadaddr} ${bootdir}/${bootfile}\0" \
	"loadfdt=load ${devtype} ${bootpart} ${fdtaddr} ${bootdir}/${fdtfile}\0" \
	"mmcsearch=if mmc dev 0; then " \
			"setenv mmcdev 0; " \
		"else " \
			"setenv mmcdev 1; " \
		"fi;\0" \
	"mmcboot=run mmcsearch; " \
		"mmc dev ${mmcdev}; " \
		"setenv bootpart ${mmcdev}:2; "\
		"setenv finduuid part uuid mmc ${mmcdev}:2 uuid; "\
		"setenv devnum ${mmcdev}; " \
		"setenv devtype mmc; " \
		"if mmc rescan; then " \
			"echo SD/MMC found on device ${devnum};" \
			"if run loadbootenv; then " \
				"echo Loaded environment from ${bootenv};" \
				"run importbootenv;" \
			"fi;" \
			"if test -n $uenvcmd; then " \
				"echo Running uenvcmd ...;" \
				"run uenvcmd;" \
			"fi;" \
			"if run loadimage; then " \
				"run loadfdt; " \
				"echo Booting from mmc${mmcdev} ...; " \
				"run args_mmc; " \
				"bootz ${loadaddr} - ${fdtaddr}; " \
			"fi;" \
		"fi;\0" \
	"usbboot=" \
		"setenv devnum ${usbdev}; " \
		"setenv devtype usb; " \
		"usb start ${usbdev}; " \
		"if usb dev ${usbdev}; then " \
			"if run loadbootenv; then " \
				"echo Loaded environment from ${bootenv};" \
				"run importbootenv;" \
			"fi;" \
			"if test -n $uenvcmd; then " \
				"echo Running uenvcmd ...;" \
				"run uenvcmd;" \
			"fi;" \
			"if run loadimage; then " \
				"run loadfdt; " \
				"echo Booting from usb ${usbdev}...; " \
				"run usbargs;" \
				"bootz ${loadaddr} - ${fdtaddr}; " \
			"fi;" \
		"fi\0" \
		"fi;" \
		"usb stop ${usbdev};\0" \
	"findfdt="\
		"if test $board_name = poblano; then " \
			"setenv fdtfile am437x-poblano.dtb; fi; " \
		"if test $fdtfile = undefined; then " \
			"echo WARNING: Could not determine device tree; fi; \0" \
	NETARGS \
	DFUARGS \

#define CONFIG_BOOTCOMMAND \
	"run findfdt; " \
	"run mmcboot;" \
	"run usbboot;" \

#endif

#ifndef CONFIG_SPL_BUILD
/* CPSW Ethernet */
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_PING
#define CONFIG_CMD_MII
#define CONFIG_MII
#define CONFIG_BOOTP_DEFAULT
#define CONFIG_BOOTP_DNS
#define CONFIG_BOOTP_DNS2
#define CONFIG_BOOTP_SEND_HOSTNAME
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_SUBNETMASK
#define CONFIG_NET_RETRY_COUNT		10
#define CONFIG_PHY_GIGE
#endif

#define CONFIG_DRIVER_TI_CPSW
#define CONFIG_PHYLIB
#define PHY_ANEG_TIMEOUT	8000 /* PHY needs longer aneg time at 1G */

#define CONFIG_SPL_ENV_SUPPORT
#define CONFIG_SPL_NET_VCI_STRING	"AM43xx U-Boot SPL"

#if defined(CONFIG_SPL_BUILD) && defined(CONFIG_SPL_ETH_SUPPORT)
#undef CONFIG_ENV_IS_IN_FAT
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_SPL_NET_SUPPORT
#endif

#define CONFIG_SYS_RX_ETH_BUFFER	64

#endif	/* __CONFIG_POBLANO_H */
