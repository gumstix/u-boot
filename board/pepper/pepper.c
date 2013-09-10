/*
 * pepper.c
 *
 * Board functions for Gumstix Pepper Single Board Computer
 *
 * Copyright (C) 2013, Gumstix, Incorporated - http://www.gumstix.com/
 * Based on board/ti/am335x/board.c from Texas Instruments, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR /PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <common.h>
#include <errno.h>
#include <spl.h>
#include <asm/arch/cpu.h>
#include <asm/arch/hardware.h>
#include <asm/arch/omap.h>
#include <asm/arch/ddr_defs.h>
#include <asm/arch/clock.h>
#include <asm/arch/gpio.h>
#include <asm/arch/mmc_host_def.h>
#include <asm/arch/sys_proto.h>
#include <asm/io.h>
#include <asm/emif.h>
#include <asm/gpio.h>
#include <i2c.h>
#include <miiphy.h>
#include <cpsw.h>

#include <asm/arch/mux.h>

static struct module_pin_mux uart0_pin_mux[] = {
	{OFFSET(uart0_rxd), (MODE(0) | PULLUP_EN | RXACTIVE)},	/* UART0_RXD */
	{OFFSET(uart0_txd), (MODE(0) | PULLUDEN)},		/* UART0_TXD */
	{-1},
};

static struct module_pin_mux mmc0_pin_mux[] = {
	{OFFSET(mmc0_dat3), (MODE(0) | RXACTIVE | PULLUP_EN)},	/* MMC0_DAT3 */
	{OFFSET(mmc0_dat2), (MODE(0) | RXACTIVE | PULLUP_EN)},	/* MMC0_DAT2 */
	{OFFSET(mmc0_dat1), (MODE(0) | RXACTIVE | PULLUP_EN)},	/* MMC0_DAT1 */
	{OFFSET(mmc0_dat0), (MODE(0) | RXACTIVE | PULLUP_EN)},	/* MMC0_DAT0 */
	{OFFSET(mmc0_clk), (MODE(0) | RXACTIVE | PULLUP_EN)},	/* MMC0_CLK */
	{OFFSET(mmc0_cmd), (MODE(0) | RXACTIVE | PULLUP_EN)},	/* MMC0_CMD */
	{OFFSET(spi0_cs1), (MODE(5) | RXACTIVE | PULLUP_EN)},	/* MMC0_CD */
	{-1},
};

static struct module_pin_mux i2c0_pin_mux[] = {
	{OFFSET(i2c0_sda), (MODE(0) | RXACTIVE | PULLUDEN | SLEWCTRL)}, /* I2C_DATA */
	{OFFSET(i2c0_scl), (MODE(0) | RXACTIVE | PULLUDEN | SLEWCTRL)}, /* I2C_SCLK */
	{-1},
};

static struct module_pin_mux rgmii1_pin_mux[] = {
	{OFFSET(mii1_txen), MODE(2)},			/* RGMII1_TCTL */
	{OFFSET(mii1_rxdv), MODE(2) | RXACTIVE},	/* RGMII1_RCTL */
	{OFFSET(mii1_txd3), MODE(2)},			/* RGMII1_TD3 */
	{OFFSET(mii1_txd2), MODE(2)},			/* RGMII1_TD2 */
	{OFFSET(mii1_txd1), MODE(2)},			/* RGMII1_TD1 */
	{OFFSET(mii1_txd0), MODE(2)},			/* RGMII1_TD0 */
	{OFFSET(mii1_txclk), MODE(2)},			/* RGMII1_TCLK */
	{OFFSET(mii1_rxclk), MODE(2) | RXACTIVE},	/* RGMII1_RCLK */
	{OFFSET(mii1_rxd3), MODE(2) | RXACTIVE},	/* RGMII1_RD3 */
	{OFFSET(mii1_rxd2), MODE(2) | RXACTIVE},	/* RGMII1_RD2 */
	{OFFSET(mii1_rxd1), MODE(2) | RXACTIVE},	/* RGMII1_RD1 */
	{OFFSET(mii1_rxd0), MODE(2) | RXACTIVE},	/* RGMII1_RD0 */
	{OFFSET(mdio_data), MODE(0) | RXACTIVE | PULLUP_EN},/* MDIO_DATA */
	{OFFSET(mdio_clk), MODE(0) | PULLUP_EN},	/* MDIO_CLK */
	{OFFSET(rmii1_refclk), MODE(7) | RXACTIVE},	/* ETH_INT */
	{OFFSET(mii1_col), MODE(7) | PULLUP_EN},	/* PHY_NRESET */
	{-1},
};

DECLARE_GLOBAL_DATA_PTR;

static struct wd_timer *wdtimer = (struct wd_timer *)WDT_BASE;

/* MII mode defines */
#define RGMII_MODE_ENABLE	0x3A

static struct ctrl_dev *cdev = (struct ctrl_dev *)CTRL_DEVICE_BASE;


#ifdef CONFIG_SPL_BUILD
static const struct ddr_data ddr2_data = {
	.datardsratio0 = ((MT47H128M16RT25E_RD_DQS<<30) |
			  (MT47H128M16RT25E_RD_DQS<<20) |
			  (MT47H128M16RT25E_RD_DQS<<10) |
			  (MT47H128M16RT25E_RD_DQS<<0)),
	.datawdsratio0 = ((MT47H128M16RT25E_WR_DQS<<30) |
			  (MT47H128M16RT25E_WR_DQS<<20) |
			  (MT47H128M16RT25E_WR_DQS<<10) |
			  (MT47H128M16RT25E_WR_DQS<<0)),
	.datawiratio0 = ((MT47H128M16RT25E_PHY_WRLVL<<30) |
			 (MT47H128M16RT25E_PHY_WRLVL<<20) |
			 (MT47H128M16RT25E_PHY_WRLVL<<10) |
			 (MT47H128M16RT25E_PHY_WRLVL<<0)),
	.datagiratio0 = ((MT47H128M16RT25E_PHY_GATELVL<<30) |
			 (MT47H128M16RT25E_PHY_GATELVL<<20) |
			 (MT47H128M16RT25E_PHY_GATELVL<<10) |
			 (MT47H128M16RT25E_PHY_GATELVL<<0)),
	.datafwsratio0 = ((MT47H128M16RT25E_PHY_FIFO_WE<<30) |
			  (MT47H128M16RT25E_PHY_FIFO_WE<<20) |
			  (MT47H128M16RT25E_PHY_FIFO_WE<<10) |
			  (MT47H128M16RT25E_PHY_FIFO_WE<<0)),
	.datawrsratio0 = ((MT47H128M16RT25E_PHY_WR_DATA<<30) |
			  (MT47H128M16RT25E_PHY_WR_DATA<<20) |
			  (MT47H128M16RT25E_PHY_WR_DATA<<10) |
			  (MT47H128M16RT25E_PHY_WR_DATA<<0)),
	.datauserank0delay = MT47H128M16RT25E_PHY_RANK0_DELAY,
	.datadldiff0 = PHY_DLL_LOCK_DIFF,
};

static const struct cmd_control ddr2_cmd_ctrl_data = {
	.cmd0csratio = MT47H128M16RT25E_RATIO,
	.cmd0dldiff = MT47H128M16RT25E_DLL_LOCK_DIFF,
	.cmd0iclkout = MT47H128M16RT25E_INVERT_CLKOUT,

	.cmd1csratio = MT47H128M16RT25E_RATIO,
	.cmd1dldiff = MT47H128M16RT25E_DLL_LOCK_DIFF,
	.cmd1iclkout = MT47H128M16RT25E_INVERT_CLKOUT,

	.cmd2csratio = MT47H128M16RT25E_RATIO,
	.cmd2dldiff = MT47H128M16RT25E_DLL_LOCK_DIFF,
	.cmd2iclkout = MT47H128M16RT25E_INVERT_CLKOUT,
};

static const struct emif_regs ddr2_emif_reg_data = {
	.sdram_config = MT47H128M16RT25E_EMIF_SDCFG,
	.ref_ctrl = MT47H128M16RT25E_EMIF_SDREF,
	.sdram_tim1 = MT47H128M16RT25E_EMIF_TIM1,
	.sdram_tim2 = MT47H128M16RT25E_EMIF_TIM2,
	.sdram_tim3 = MT47H128M16RT25E_EMIF_TIM3,
	.emif_ddr_phy_ctlr_1 = MT47H128M16RT25E_EMIF_READ_LATENCY,
};
#endif

/*
 * early system init of muxing and clocks.
 */
void s_init(void)
{
	/*
	 * Save the boot parameters passed from romcode.
	 * We cannot delay the saving further than this,
	 * to prevent overwrites.
	 */
#ifdef CONFIG_SPL_BUILD
	save_omap_boot_params();
#endif

	/* WDT1 is already running when the bootloader gets control
	 * Disable it to avoid "random" resets
	 */
	writel(0xAAAA, &wdtimer->wdtwspr);
	while (readl(&wdtimer->wdtwwps) != 0x0)
		;
	writel(0x5555, &wdtimer->wdtwspr);
	while (readl(&wdtimer->wdtwwps) != 0x0)
		;

#ifdef CONFIG_SPL_BUILD
	/* Setup the PLLs and the clocks for the peripherals */
	pll_init();

	/* Should enable RTC32K clock, but this doesn't work on all boards
	rtc32k_enable();
	*/

	configure_module_pin_mux(uart0_pin_mux);

	uart_soft_reset();

	gd = &gdata;

	preloader_console_init();

	config_ddr(266, MT47H128M16RT25E_IOCTRL_VALUE, &ddr2_data, &ddr2_cmd_ctrl_data, &ddr2_emif_reg_data, 0);
#endif
}

int board_init(void)
{
	i2c_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);
	gd->bd->bi_boot_params = PHYS_DRAM_1 + 0x100;

	gpmc_init();
	configure_module_pin_mux(i2c0_pin_mux);
	configure_module_pin_mux(mmc0_pin_mux);
	configure_module_pin_mux(rgmii1_pin_mux);

	return 0;
}

static void cpsw_control(int enabled)
{
	/* VTP can be added here */
	return;
}

static struct cpsw_slave_data cpsw_slaves[] = {
	{
		.slave_reg_ofs	= 0x208,
		.sliver_reg_ofs	= 0xd80,
		.phy_id		= 0,
	},
	{
		.slave_reg_ofs	= 0x308,
		.sliver_reg_ofs	= 0xdc0,
		.phy_id		= 1,
	},
};

static struct cpsw_platform_data cpsw_data = {
	.mdio_base		= CPSW_MDIO_BASE,
	.cpsw_base		= CPSW_BASE,
	.mdio_div		= 0xff,
	.channels		= 8,
	.cpdma_reg_ofs		= 0x800,
	.slaves			= 1,
	.slave_data		= cpsw_slaves,
	.ale_reg_ofs		= 0xd00,
	.ale_entries		= 1024,
	.host_port_reg_ofs	= 0x108,
	.hw_stats_reg_ofs	= 0x900,
	.mac_control		= (1 << 5),
	.control		= cpsw_control,
	.host_port_num		= 0,
	.version		= CPSW_CTRL_VERSION_2,
};

int board_eth_init(bd_t *bis)
{
	int rv, n = 0;
	uint8_t mac_addr[6];
	uint32_t mac_hi, mac_lo;
	const char *devname;

	/* try reading mac address from efuse */
	mac_lo = readl(&cdev->macid0l);
	mac_hi = readl(&cdev->macid0h);
	mac_addr[0] = mac_hi & 0xFF;
	mac_addr[1] = (mac_hi & 0xFF00) >> 8;
	mac_addr[2] = (mac_hi & 0xFF0000) >> 16;
	mac_addr[3] = (mac_hi & 0xFF000000) >> 24;
	mac_addr[4] = mac_lo & 0xFF;
	mac_addr[5] = (mac_lo & 0xFF00) >> 8;


	writel(RGMII_MODE_ENABLE, &cdev->miisel);
	cpsw_slaves[0].phy_if = cpsw_slaves[1].phy_if = PHY_INTERFACE_MODE_RGMII;

	rv = cpsw_register(&cpsw_data);
	if (rv < 0)
		printf("Error %d registering CPSW switch\n", rv);
	else
		n += rv;

	/*
	 *
	 * CPSW RGMII Internal Delay Mode is not supported in all PVT
	 * operating points.  So we must set the TX clock delay feature
	 * in the KSZ9021 PHY.  Since we only support a single ethernet
	 * device in U-Boot, we only do this for the first instance.
	 */
	devname = miiphy_get_current_dev();
	/* max rx/tx clock delay, min rx/tx control delay */
	miiphy_write(devname, 0x0, 0x0b, 0x8104);
	miiphy_write(devname, 0x0, 0xc, 0xa0a0);

	/* min rx data delay */
	miiphy_write(devname, 0x0, 0x0b, 0x8105);
	miiphy_write(devname, 0x0, 0x0c, 0x0000);

	/* min tx data delay */
	miiphy_write(devname, 0x0, 0x0b, 0x8106);
	miiphy_write(devname, 0x0, 0x0c, 0x0000);
}
