/*
 * board.c
 *
 * Board functions for TI AM43XX based boards
 *
 * Copyright (C) 2013, Texas Instruments, Incorporated - http://www.ti.com/
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <i2c.h>
#include <asm/errno.h>
#include <spl.h>
#include <usb.h>
#include <asm/arch/clock.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/mux.h>
#include <asm/arch/ddr_defs.h>
#include <asm/arch/gpio.h>
#include <asm/emif.h>
#include "board.h"
#include <power/pmic.h>
#include <power/tps65218.h>
#include <power/tps62362.h>
#include <miiphy.h>
#include <cpsw.h>
#include <linux/usb/gadget.h>
#include <dwc3-uboot.h>
#include <dwc3-omap-uboot.h>
#include <ti-usb-phy-uboot.h>

DECLARE_GLOBAL_DATA_PTR;

static struct ctrl_dev *cdev = (struct ctrl_dev *)CTRL_DEVICE_BASE;

#ifndef CONFIG_SKIP_LOWLEVEL_INIT

const struct dpll_params dpll_mpu  = { 800, 23,  1, -1, -1, -1, -1};
const struct dpll_params dpll_core = {1000, 23, -1, -1, 10,  8,  4};
const struct dpll_params dpll_per  = { 400,  9,  5, -1, -1, -1, -1};
const struct dpll_params dpll_ddr  = {  50,  2,  1, -1,  2, -1, -1};

const struct ctrl_ioregs ioregs_ddr3 = {
	.cm0ioctl		= DDR3_ADDRCTRL_IOCTRL_VALUE,
	.cm1ioctl		= DDR3_ADDRCTRL_WD0_IOCTRL_VALUE,
	.cm2ioctl		= DDR3_ADDRCTRL_WD1_IOCTRL_VALUE,
	.dt0ioctl		= DDR3_DATA0_IOCTRL_VALUE,
	.dt1ioctl		= DDR3_DATA0_IOCTRL_VALUE,
	.dt2ioctrl		= DDR3_DATA0_IOCTRL_VALUE,
	.dt3ioctrl		= DDR3_DATA0_IOCTRL_VALUE,
	.emif_sdram_config_ext	= 0xc163,
};

static const struct emif_regs ddr3_sk_emif_regs_400Mhz = {
	.sdram_config			= 0x638453b2,
	.sdram_config2			= 0x00000000,
	.ref_ctrl			= 0x00000c30,
	.sdram_tim1			= 0xeaaad4db,
	.sdram_tim2			= 0x266b7fda,
	.sdram_tim3			= 0x107f8678,
	.read_idle_ctrl			= 0x00050000,
	.zq_config			= 0x50074be4,
	.temp_alert_config		= 0x0,
	.emif_ddr_phy_ctlr_1		= 0x0e084008,
	.emif_ddr_ext_phy_ctrl_1	= 0x08020080,
	.emif_ddr_ext_phy_ctrl_2	= 0x89,
	.emif_ddr_ext_phy_ctrl_3	= 0x90,
	.emif_ddr_ext_phy_ctrl_4	= 0x8e,
	.emif_ddr_ext_phy_ctrl_5	= 0x8d,
	.emif_rd_wr_lvl_rmp_win		= 0x0,
	.emif_rd_wr_lvl_rmp_ctl		= 0x00000000,
	.emif_rd_wr_lvl_ctl		= 0x00000000,
	.emif_rd_wr_exec_thresh		= 0x80000000,
	.emif_prio_class_serv_map	= 0x80000001,
	.emif_connect_id_serv_1_map	= 0x80000094,
	.emif_connect_id_serv_2_map	= 0x00000000,
	.emif_cos_config		= 0x000FFFFF
};

void emif_get_ext_phy_ctrl_const_regs(const u32 **regs, u32 *size)
{
	return;
}

const struct dpll_params *get_dpll_ddr_params(void)
{
	return &dpll_ddr;
}

const struct dpll_params *get_dpll_mpu_params(void)
{
	return &dpll_mpu;
}

const struct dpll_params *get_dpll_core_params(void)
{
	return &dpll_core;
}

const struct dpll_params *get_dpll_per_params(void)
{
	return &dpll_per;
}

void scale_vcores_generic(u32 m)
{
	int mpu_vdd;

	if (i2c_probe(TPS65218_CHIP_PM))
		return;

	switch (m) {
	case 1000:
		mpu_vdd = TPS65218_DCDC_VOLT_SEL_1330MV;
		break;
	case 800:
		mpu_vdd = TPS65218_DCDC_VOLT_SEL_1260MV;
		break;
	case 720:
		mpu_vdd = TPS65218_DCDC_VOLT_SEL_1200MV;
		break;
	case 600:
		mpu_vdd = TPS65218_DCDC_VOLT_SEL_1100MV;
		break;
	case 300:
		mpu_vdd = TPS65218_DCDC_VOLT_SEL_0950MV;
		break;
	default:
		puts("Unknown MPU clock, not scaling\n");
		return;
	}

	/* Set DCDC1 (CORE) voltage to 1.1V */
	if (tps65218_voltage_update(TPS65218_DCDC1,
				    TPS65218_DCDC_VOLT_SEL_1100MV)) {
		printf("%s failure\n", __func__);
		return;
	}

	/* Set DCDC2 (MPU) voltage */
	if (tps65218_voltage_update(TPS65218_DCDC2, mpu_vdd)) {
		printf("%s failure\n", __func__);
		return;
	}
}

void scale_vcores(void)
{
	const struct dpll_params *mpu_params;

	enable_i2c_pin_mux();
	i2c_init(CONFIG_SYS_OMAP24_I2C_SPEED, CONFIG_SYS_OMAP24_I2C_SLAVE);

	/* Get the frequency */
	mpu_params = get_dpll_mpu_params();
	scale_vcores_generic(mpu_params->m);
}

void set_uart_mux_conf(void)
{
	enable_uart0_pin_mux();
}

void set_mux_conf_regs(void)
{
	enable_board_pin_mux();
}

void sdram_init(void)
{
	config_ddr(400, &ioregs_ddr3, NULL, NULL, &ddr3_sk_emif_regs_400Mhz, 0);
}
#endif

/* setup board specific PMIC */
int power_init_board(void)
{
	struct pmic *p;

	power_tps65218_init(I2C_PMIC);
	p = pmic_get("TPS65218_PMIC");
	if (p && !pmic_probe(p))
		puts("PMIC:  TPS65218\n");

	return 0;
}

int board_init(void)
{
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;
	gpmc_init();

	return 0;
}

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
	return 0;
}
#endif

#ifdef CONFIG_USB_DWC3
static struct dwc3_device usb_otg_ss1 = {
	.maximum_speed = USB_SPEED_HIGH,
	.base = USB_OTG_SS1_BASE,
	.tx_fifo_resize = false,
	.index = 0,
};

static struct dwc3_omap_device usb_otg_ss1_glue = {
	.base = (void *)USB_OTG_SS1_GLUE_BASE,
	.utmi_mode = DWC3_OMAP_UTMI_MODE_SW,
	.index = 0,
};

static struct ti_usb_phy_device usb_phy1_device = {
	.usb2_phy_power = (void *)USB2_PHY1_POWER,
	.index = 0,
};

static struct dwc3_device usb_otg_ss2 = {
	.maximum_speed = USB_SPEED_HIGH,
	.base = USB_OTG_SS2_BASE,
	.tx_fifo_resize = false,
	.index = 1,
};

static struct dwc3_omap_device usb_otg_ss2_glue = {
	.base = (void *)USB_OTG_SS2_GLUE_BASE,
	.utmi_mode = DWC3_OMAP_UTMI_MODE_SW,
	.index = 1,
};

static struct ti_usb_phy_device usb_phy2_device = {
	.usb2_phy_power = (void *)USB2_PHY2_POWER,
	.index = 1,
};

int board_usb_init(int index, enum usb_init_type init)
{
	enable_usb_clocks(index);
	switch (index) {
	case 0:
		if (init == USB_INIT_DEVICE) {
			usb_otg_ss1.dr_mode = USB_DR_MODE_PERIPHERAL;
			usb_otg_ss1_glue.vbus_id_status = OMAP_DWC3_VBUS_VALID;
		} else {
			usb_otg_ss1.dr_mode = USB_DR_MODE_HOST;
			usb_otg_ss1_glue.vbus_id_status = OMAP_DWC3_ID_GROUND;
		}

		dwc3_omap_uboot_init(&usb_otg_ss1_glue);
		ti_usb_phy_uboot_init(&usb_phy1_device);
		dwc3_uboot_init(&usb_otg_ss1);
		break;
	case 1:
		if (init == USB_INIT_DEVICE) {
			usb_otg_ss2.dr_mode = USB_DR_MODE_PERIPHERAL;
			usb_otg_ss2_glue.vbus_id_status = OMAP_DWC3_VBUS_VALID;
		} else {
			usb_otg_ss2.dr_mode = USB_DR_MODE_HOST;
			usb_otg_ss2_glue.vbus_id_status = OMAP_DWC3_ID_GROUND;
		}

		ti_usb_phy_uboot_init(&usb_phy2_device);
		dwc3_omap_uboot_init(&usb_otg_ss2_glue);
		dwc3_uboot_init(&usb_otg_ss2);
		break;
	default:
		printf("Invalid Controller Index\n");
	}

	return 0;
}

int board_usb_cleanup(int index, enum usb_init_type init)
{
	switch (index) {
	case 0:
	case 1:
		ti_usb_phy_uboot_exit(index);
		dwc3_uboot_exit(index);
		dwc3_omap_uboot_exit(index);
		break;
	default:
		printf("Invalid Controller Index\n");
	}
	disable_usb_clocks(index);

	return 0;
}

int usb_gadget_handle_interrupts(int index)
{
	u32 status;

	status = dwc3_omap_uboot_interrupt_status(index);
	if (status)
		dwc3_uboot_handle_interrupt(index);

	return 0;
}
#endif

#ifdef CONFIG_DRIVER_TI_CPSW

static void cpsw_control(int enabled)
{
	/* Additional controls can be added here */
	return;
}

static struct cpsw_slave_data cpsw_slaves[] = {
	{
		.slave_reg_ofs	= 0x208,
		.sliver_reg_ofs	= 0xd80,
		.phy_addr		= 0,
		.phy_if 		= PHY_INTERFACE_MODE_RGMII,
	},
	{
		.slave_reg_ofs	= 0x308,
		.sliver_reg_ofs	= 0xdc0,
		.phy_addr	= 1,
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
	.bd_ram_ofs		= 0x2000,
	.mac_control		= (1 << 5),
	.control		= cpsw_control,
	.host_port_num		= 0,
	.version		= CPSW_CTRL_VERSION_2,
};

int board_eth_init(bd_t *bis)
{
	int rv;
	const char *devname;
	uint8_t mac_addr[6];
	uint32_t mac_hi, mac_lo;

	/* try reading mac address from efuse */
	mac_lo = readl(&cdev->macid0l);
	mac_hi = readl(&cdev->macid0h);
	mac_addr[0] = mac_hi & 0xFF;
	mac_addr[1] = (mac_hi & 0xFF00) >> 8;
	mac_addr[2] = (mac_hi & 0xFF0000) >> 16;
	mac_addr[3] = (mac_hi & 0xFF000000) >> 24;
	mac_addr[4] = mac_lo & 0xFF;
	mac_addr[5] = (mac_lo & 0xFF00) >> 8;

	if (!getenv("ethaddr")) {
		puts("<ethaddr> not set. Validating first E-fuse MAC\n");
		if (is_valid_ethaddr(mac_addr))
			eth_setenv_enetaddr("ethaddr", mac_addr);
	}

	mac_lo = readl(&cdev->macid1l);
	mac_hi = readl(&cdev->macid1h);
	mac_addr[0] = mac_hi & 0xFF;
	mac_addr[1] = (mac_hi & 0xFF00) >> 8;
	mac_addr[2] = (mac_hi & 0xFF0000) >> 16;
	mac_addr[3] = (mac_hi & 0xFF000000) >> 24;
	mac_addr[4] = mac_lo & 0xFF;
	mac_addr[5] = (mac_lo & 0xFF00) >> 8;

	if (!getenv("eth1addr")) {
		if (is_valid_ethaddr(mac_addr))
			eth_setenv_enetaddr("eth1addr", mac_addr);
	}

	writel(RGMII_MODE_ENABLE, &cdev->miisel);

	rv = cpsw_register(&cpsw_data);
	if (rv < 0)
		printf("Error %d registering CPSW switch\n", rv);

	/*
	 * CPSW RGMII Internal Delay Mode is not supported in all PVT
	 * operating points.  So we must set the TX clock delay feature
	 * in the KSZ9021 PHY.  Since we only support a single ethernet
	 * device in U-Boot, we only do this for the current instance.
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

	return rv;
}
#endif
