/*
 * mux.c
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <common.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/hardware.h>
#include <asm/io.h>
#include <i2c.h>

#define MUX_CFG(value, offset)	\
	__raw_writel(value, (CTRL_BASE + offset));

/* PAD Control Fields */
#define SLEWCTRL	(0x1 << 6)
#define RXACTIVE	(0x1 << 5)
#define PULLUP_EN	(0x1 << 4) /* Pull UP Selection */
#define PULLUDEN	(0x0 << 3) /* Pull up enabled */
#define PULLUDDIS	(0x1 << 3) /* Pull up disabled */
#define MODE(val)	val	/* used for Readability */

/*
 * PAD CONTROL OFFSETS
 * Field names corresponds to the pad signal name
 */
struct pad_signals {
	int gpmc_ad0;
	int gpmc_ad1;
	int gpmc_ad2;
	int gpmc_ad3;
	int gpmc_ad4;
	int gpmc_ad5;
	int gpmc_ad6;
	int gpmc_ad7;
	int gpmc_ad8;
	int gpmc_ad9;
	int gpmc_ad10;
	int gpmc_ad11;
	int gpmc_ad12;
	int gpmc_ad13;
	int gpmc_ad14;
	int gpmc_ad15;
	int gpmc_a0;
	int gpmc_a1;
	int gpmc_a2;
	int gpmc_a3;
	int gpmc_a4;
	int gpmc_a5;
	int gpmc_a6;
	int gpmc_a7;
	int gpmc_a8;
	int gpmc_a9;
	int gpmc_a10;
	int gpmc_a11;
	int gpmc_wait0;
	int gpmc_wpn;
	int gpmc_be1n;
	int gpmc_csn0;
	int gpmc_csn1;
	int gpmc_csn2;
	int gpmc_csn3;
	int gpmc_clk;
	int gpmc_advn_ale;
	int gpmc_oen_ren;
	int gpmc_wen;
	int gpmc_be0n_cle;
	int lcd_data0;
	int lcd_data1;
	int lcd_data2;
	int lcd_data3;
	int lcd_data4;
	int lcd_data5;
	int lcd_data6;
	int lcd_data7;
	int lcd_data8;
	int lcd_data9;
	int lcd_data10;
	int lcd_data11;
	int lcd_data12;
	int lcd_data13;
	int lcd_data14;
	int lcd_data15;
	int lcd_vsync;
	int lcd_hsync;
	int lcd_pclk;
	int lcd_ac_bias_en;
	int mmc0_dat3;
	int mmc0_dat2;
	int mmc0_dat1;
	int mmc0_dat0;
	int mmc0_clk;
	int mmc0_cmd;
	int mii1_col;
	int mii1_crs;
	int mii1_rxerr;
	int mii1_txen;
	int mii1_rxdv;
	int mii1_txd3;
	int mii1_txd2;
	int mii1_txd1;
	int mii1_txd0;
	int mii1_txclk;
	int mii1_rxclk;
	int mii1_rxd3;
	int mii1_rxd2;
	int mii1_rxd1;
	int mii1_rxd0;
	int rmii1_refclk;
	int mdio_data;
	int mdio_clk;
	int spi0_sclk;
	int spi0_d0;
	int spi0_d1;
	int spi0_cs0;
	int spi0_cs1;
	int ecap0_in_pwm0_out;
	int uart0_ctsn;
	int uart0_rtsn;
	int uart0_rxd;
	int uart0_txd;
	int uart1_ctsn;
	int uart1_rtsn;
	int uart1_rxd;
	int uart1_txd;
	int i2c0_sda;
	int i2c0_scl;
	int mcasp0_aclkx;
	int mcasp0_fsx;
	int mcasp0_axr0;
	int mcasp0_ahclkr;
	int mcasp0_aclkr;
	int mcasp0_fsr;
	int mcasp0_axr1;
	int mcasp0_ahclkx;
	int xdma_event_intr0;
	int xdma_event_intr1;
	int nresetin_out;
	int porz;
	int nnmi;
	int osc0_in;
	int osc0_out;
	int rsvd1;
	int tms;
	int tdi;
	int tdo;
	int tck;
	int ntrst;
	int emu0;
	int emu1;
	int osc1_in;
	int osc1_out;
	int pmic_power_en;
	int rtc_porz;
	int rsvd2;
	int ext_wakeup;
	int enz_kaldo_1p8v;
	int usb0_dm;
	int usb0_dp;
	int usb0_ce;
	int usb0_id;
	int usb0_vbus;
	int usb0_drvvbus;
	int usb1_dm;
	int usb1_dp;
	int usb1_ce;
	int usb1_id;
	int usb1_vbus;
	int usb1_drvvbus;
	int ddr_resetn;
	int ddr_csn0;
	int ddr_cke;
	int ddr_ck;
	int ddr_nck;
	int ddr_casn;
	int ddr_rasn;
	int ddr_wen;
	int ddr_ba0;
	int ddr_ba1;
	int ddr_ba2;
	int ddr_a0;
	int ddr_a1;
	int ddr_a2;
	int ddr_a3;
	int ddr_a4;
	int ddr_a5;
	int ddr_a6;
	int ddr_a7;
	int ddr_a8;
	int ddr_a9;
	int ddr_a10;
	int ddr_a11;
	int ddr_a12;
	int ddr_a13;
	int ddr_a14;
	int ddr_a15;
	int ddr_odt;
	int ddr_d0;
	int ddr_d1;
	int ddr_d2;
	int ddr_d3;
	int ddr_d4;
	int ddr_d5;
	int ddr_d6;
	int ddr_d7;
	int ddr_d8;
	int ddr_d9;
	int ddr_d10;
	int ddr_d11;
	int ddr_d12;
	int ddr_d13;
	int ddr_d14;
	int ddr_d15;
	int ddr_dqm0;
	int ddr_dqm1;
	int ddr_dqs0;
	int ddr_dqsn0;
	int ddr_dqs1;
	int ddr_dqsn1;
	int ddr_vref;
	int ddr_vtp;
	int ddr_strben0;
	int ddr_strben1;
	int ain7;
	int ain6;
	int ain5;
	int ain4;
	int ain3;
	int ain2;
	int ain1;
	int ain0;
	int vrefp;
	int vrefn;
};

struct module_pin_mux {
	short reg_offset;
	unsigned char val;
};

/* Pad control register offset */
#define PAD_CTRL_BASE	0x800
#define OFFSET(x)	(unsigned int) (&((struct pad_signals *) \
				(PAD_CTRL_BASE))->x)

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
	{OFFSET(mcasp0_aclkr), (MODE(4) | RXACTIVE)},		/* MMC0_WP */
	{OFFSET(spi0_cs1), (MODE(5) | RXACTIVE | PULLUP_EN)},	/* MMC0_CD */
	{-1},
};

static struct module_pin_mux mmc0_pin_mux_sk_evm[] = {
	{OFFSET(mmc0_dat3), (MODE(0) | RXACTIVE | PULLUP_EN)},	/* MMC0_DAT3 */
	{OFFSET(mmc0_dat2), (MODE(0) | RXACTIVE | PULLUP_EN)},	/* MMC0_DAT2 */
	{OFFSET(mmc0_dat1), (MODE(0) | RXACTIVE | PULLUP_EN)},	/* MMC0_DAT1 */
	{OFFSET(mmc0_dat0), (MODE(0) | RXACTIVE | PULLUP_EN)},	/* MMC0_DAT0 */
	{OFFSET(mmc0_clk), (MODE(0) | RXACTIVE | PULLUP_EN)},	/* MMC0_CLK */
	{OFFSET(mmc0_cmd), (MODE(0) | RXACTIVE | PULLUP_EN)},	/* MMC0_CMD */
	{OFFSET(spi0_cs1), (MODE(5) | RXACTIVE | PULLUP_EN)},	/* MMC0_CD */
	{-1},
};

static struct module_pin_mux mmc1hi_pin_mux[] = {
	{OFFSET(gpmc_ad7), (MODE(1) | RXACTIVE | PULLUP_EN)},	/* MMC1_DAT7 */
	{OFFSET(gpmc_ad6), (MODE(1) | RXACTIVE | PULLUP_EN)},	/* MMC1_DAT6 */
	{OFFSET(gpmc_ad5), (MODE(1) | RXACTIVE | PULLUP_EN)},	/* MMC1_DAT5 */
	{OFFSET(gpmc_ad4), (MODE(1) | RXACTIVE | PULLUP_EN)},	/* MMC1_DAT4 */
	{-1},
};

static struct module_pin_mux mmc1_pin_mux[] = {
	{OFFSET(gpmc_ad3), (MODE(1) | RXACTIVE | PULLUP_EN)},	/* MMC1_DAT3 */
	{OFFSET(gpmc_ad2), (MODE(1) | RXACTIVE | PULLUP_EN)},	/* MMC1_DAT2 */
	{OFFSET(gpmc_ad1), (MODE(1) | RXACTIVE | PULLUP_EN)},	/* MMC1_DAT1 */
	{OFFSET(gpmc_ad0), (MODE(1) | RXACTIVE | PULLUP_EN)},	/* MMC1_DAT0 */
	{OFFSET(gpmc_csn1), (MODE(2) | RXACTIVE | PULLUP_EN)},	/* MMC1_CLK */
	{OFFSET(gpmc_csn2), (MODE(2) | RXACTIVE | PULLUP_EN)},	/* MMC1_CMD */
	{OFFSET(gpmc_csn0), (MODE(7) | RXACTIVE | PULLUP_EN)},	/* MMC1_WP */
	{OFFSET(gpmc_advn_ale), (MODE(7) | RXACTIVE | PULLUP_EN)},	/* MMC1_CD */
	{-1},
};

static struct module_pin_mux mmc2_pin_mux[] = {
	{OFFSET(gpmc_be1n), (MODE(3) | RXACTIVE | PULLUP_EN )},	/* SDIO2_DAT3 */
	{OFFSET(gpmc_a3), (MODE(3) | RXACTIVE | PULLUP_EN)},	/* SDIO2_DAT2 */
	{OFFSET(gpmc_a2), (MODE(3) | RXACTIVE | PULLUP_EN)},	/* SDIO2_DAT1 */
	{OFFSET(gpmc_a1), (MODE(3) | RXACTIVE | PULLUP_EN)},	/* SDIO2_DAT0 */
	{OFFSET(gpmc_clk), (MODE(3) | RXACTIVE | PULLUP_EN)},	/* SDIO2_CLK */
	{OFFSET(gpmc_csn3), (MODE(3) | RXACTIVE | PULLUP_EN)},	/* SDIO2_CMD */
	{-1},
};

static struct module_pin_mux i2c0_pin_mux[] = {
	{OFFSET(i2c0_sda), (MODE(0) | RXACTIVE |
			PULLUDEN | SLEWCTRL)}, /* I2C_DATA */
	{OFFSET(i2c0_scl), (MODE(0) | RXACTIVE |
			PULLUDEN | SLEWCTRL)}, /* I2C_SCLK */
	{-1},
};

static struct module_pin_mux i2c1_pin_mux[] = {
	{OFFSET(spi0_d1), (MODE(2) | RXACTIVE |
			PULLUDEN | SLEWCTRL)},	/* I2C_DATA */
	{OFFSET(spi0_cs0), (MODE(2) | RXACTIVE |
			PULLUDEN | SLEWCTRL)},	/* I2C_SCLK */
	{-1},
};

static struct module_pin_mux i2c1_pin_mux_pepper[] = {
	{OFFSET(mii1_crs), (MODE(3) | RXACTIVE |
			PULLUP_EN | PULLUDEN | SLEWCTRL)},	/* I2C_DATA */
	{OFFSET(mii1_rxerr), (MODE(3) | RXACTIVE |
			PULLUP_EN | PULLUDEN | SLEWCTRL)},	/* I2C_SCLK */
	{-1},
};

static struct module_pin_mux spi0_pin_mux[] = {
	{OFFSET(spi0_sclk), (MODE(0) | RXACTIVE | PULLUDEN)},	/* SPI0_SCLK */
	{OFFSET(spi0_d0), (MODE(0) | RXACTIVE |
			PULLUDEN | PULLUP_EN)},			/* SPI0_D0 */
	{OFFSET(spi0_d1), (MODE(0) | RXACTIVE | PULLUDEN)},	/* SPI0_D1 */
	{OFFSET(spi0_cs0), (MODE(0) | RXACTIVE |
			PULLUDEN | PULLUP_EN)},			/* SPI0_CS0 */
	{-1},
};

static struct module_pin_mux gpio0_7_pin_mux[] = {
	{OFFSET(ecap0_in_pwm0_out), (MODE(7) | PULLUDEN)},	/* GPIO0_7 */
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
	{-1},
};

static struct module_pin_mux mii1_pin_mux[] = {
	{OFFSET(mii1_rxerr), MODE(0) | RXACTIVE},	/* MII1_RXERR */
	{OFFSET(mii1_txen), MODE(0)},			/* MII1_TXEN */
	{OFFSET(mii1_rxdv), MODE(0) | RXACTIVE},	/* MII1_RXDV */
	{OFFSET(mii1_txd3), MODE(0)},			/* MII1_TXD3 */
	{OFFSET(mii1_txd2), MODE(0)},			/* MII1_TXD2 */
	{OFFSET(mii1_txd1), MODE(0)},			/* MII1_TXD1 */
	{OFFSET(mii1_txd0), MODE(0)},			/* MII1_TXD0 */
	{OFFSET(mii1_txclk), MODE(0) | RXACTIVE},	/* MII1_TXCLK */
	{OFFSET(mii1_rxclk), MODE(0) | RXACTIVE},	/* MII1_RXCLK */
	{OFFSET(mii1_rxd3), MODE(0) | RXACTIVE},	/* MII1_RXD3 */
	{OFFSET(mii1_rxd2), MODE(0) | RXACTIVE},	/* MII1_RXD2 */
	{OFFSET(mii1_rxd1), MODE(0) | RXACTIVE},	/* MII1_RXD1 */
	{OFFSET(mii1_rxd0), MODE(0) | RXACTIVE},	/* MII1_RXD0 */
	{OFFSET(mdio_data), MODE(0) | RXACTIVE | PULLUP_EN}, /* MDIO_DATA */
	{OFFSET(mdio_clk), MODE(0) | PULLUP_EN},	/* MDIO_CLK */
	{-1},
};

static struct module_pin_mux asp0_pin_mux[] = {
	{OFFSET(mcasp0_aclkx), (MODE(0) | RXACTIVE | PULLUP_EN)},/* MCASP0_ACLKX */
	{OFFSET(mcasp0_fsx), (MODE(0) | RXACTIVE | PULLUP_EN)},	/* MCASP0_FSX */
	{OFFSET(mcasp0_axr0), (MODE(0) | RXACTIVE | PULLUP_EN)},/* MCASP0_AXR0 */
	{OFFSET(mcasp0_ahclkr), (MODE(0) | RXACTIVE | PULLUP_EN)},/* MCASP0_AHCLKR */
	{OFFSET(mcasp0_aclkr), (MODE(0) | RXACTIVE | PULLUP_EN)},/* MCASP0_ACLKR */
	{OFFSET(mcasp0_fsr), (MODE(0) | RXACTIVE | PULLUP_EN)},	/* MCASP0_FSR */
	{OFFSET(mcasp0_axr1), (MODE(0) | RXACTIVE | PULLUP_EN)},/* MCASP0_AXR1 */
	{OFFSET(mcasp0_ahclkx), (MODE(0) | RXACTIVE | PULLUP_EN)},/* MCASP0_AHCLKX */
	{-1},
};

static struct module_pin_mux lcd_pin_mux[] = {
	{OFFSET(gpmc_ad8), (MODE(1))},				/* LCD_DATA23 */
	{OFFSET(gpmc_ad9), (MODE(1))},				/* LCD_DATA22 */
	{OFFSET(gpmc_ad10), (MODE(1))},				/* LCD_DATA21 */
	{OFFSET(gpmc_ad11), (MODE(1))},				/* LCD_DATA20 */
	{OFFSET(gpmc_ad12), (MODE(1))},				/* LCD_DATA19 */
	{OFFSET(gpmc_ad13), (MODE(1))},				/* LCD_DATA18 */
	{OFFSET(gpmc_ad14), (MODE(1))},				/* LCD_DATA17 */
	{OFFSET(gpmc_ad15), (MODE(1))},				/* LCD_DATA16 */
	{OFFSET(lcd_data0), (MODE(0))},				/* LCD_DATA0 */
	{OFFSET(lcd_data1), (MODE(0))},				/* LCD_DATA1 */
	{OFFSET(lcd_data2), (MODE(0))},				/* LCD_DATA2 */
	{OFFSET(lcd_data3), (MODE(0))},				/* LCD_DATA3 */
	{OFFSET(lcd_data4), (MODE(0))},				/* LCD_DATA4 */
	{OFFSET(lcd_data5), (MODE(0))},				/* LCD_DATA5 */
	{OFFSET(lcd_data6), (MODE(0))},				/* LCD_DATA6 */
	{OFFSET(lcd_data7), (MODE(0))},				/* LCD_DATA7 */
	{OFFSET(lcd_data8), (MODE(0))},				/* LCD_DATA8*/
	{OFFSET(lcd_data9), (MODE(0))},				/* LCD_DATA9 */
	{OFFSET(lcd_data10), (MODE(0))},			/* LCD_DATA10 */
	{OFFSET(lcd_data11), (MODE(0))},			/* LCD_DATA11 */
	{OFFSET(lcd_data12), (MODE(0))},			/* LCD_DATA12 */
	{OFFSET(lcd_data13), (MODE(0))},			/* LCD_DATA13 */
	{OFFSET(lcd_data14), (MODE(0))},			/* LCD_DATA14 */
	{OFFSET(lcd_data15), (MODE(0))},			/* LCD_DATA15 */
	{OFFSET(lcd_vsync),  (MODE(0))},			/* LCD_VSYNC */
	{OFFSET(lcd_hsync),  (MODE(0))},			/* LCD_HSYNC */
	{OFFSET(lcd_pclk),   (MODE(0))},			/* LCD_PCLK */
	{OFFSET(lcd_ac_bias_en), (MODE(0))},			/* LCD_AC_BIAS_EN */
	{-1},
};

static struct module_pin_mux pepper_pin_mux[] = {
	{OFFSET(ecap0_in_pwm0_out), (MODE(7))},			/* GPIO7 */
	{OFFSET(xdma_event_intr0), (MODE(3))},			/* CLK_26M */
	{OFFSET(xdma_event_intr1), (MODE(3))},			/* CLK_32K */
	{OFFSET(gpmc_a0), (MODE(7) | PULLUP_EN | PULLUDEN)},	/* GPIO48 - audio_nreset */
	{OFFSET(gpmc_a4), (MODE(7) | PULLUP_EN | PULLUDEN)},	/* GPIO52 */
	{OFFSET(gpmc_a5), (MODE(7) | PULLUP_EN | PULLUDEN)},	/* GPIO53 */
	{OFFSET(gpmc_a6), (MODE(7) | PULLUP_EN | PULLUDEN)},	/* GPIO54 */
	{OFFSET(gpmc_a7), (MODE(7) | PULLUP_EN | PULLUDEN)},	/* GPIO55 */
	{OFFSET(gpmc_a8), (MODE(7) | PULLUP_EN | PULLUDEN)},	/* GPIO56 - wlan_nreset */
	{OFFSET(gpmc_a9), (MODE(7) | PULLUP_EN | PULLUDEN)},	/* GPIO57 */
	{OFFSET(gpmc_a10), (MODE(7) | PULLUP_EN | PULLUDEN)},	/* GPIO58 */
	{OFFSET(gpmc_a11), (MODE(7) | PULLUP_EN | PULLUDEN)},	/* GPIO59 - lcd_en */
	{OFFSET(gpmc_csn1), (MODE(7) | PULLUP_EN | PULLUDEN)},	/* GPIO62 */
	{OFFSET(gpmc_csn2), (MODE(7) | PULLUP_EN | PULLUDEN)},	/* GPIO63 */
	{OFFSET(gpmc_wen), (MODE(7) | PULLUP_EN | PULLUDEN)},	/* GPIO68 - accel_nint */
	{OFFSET(gpmc_wait0), (MODE(7) | PULLUP_EN | PULLUDEN)},	/* GPIO30 - wlan_npwrdn */
	{OFFSET(gpmc_wpn), (MODE(7) | PULLUP_EN | PULLUDEN)},	/* GPIO31 - emmc_nreset */
	{OFFSET(mii1_col), (MODE(7) | PULLUP_EN | PULLUDEN)},	/* GPIO96 - eth_nreset */
	{OFFSET(rmii1_refclk), (MODE(7) | PULLUP_EN | PULLUDEN)},/* GPIO29 - eth_nint */
	{-1},
};

/*
 * Configure the pin mux for the module
 */
static void configure_module_pin_mux(struct module_pin_mux *mod_pin_mux)
{
	int i;

	if (!mod_pin_mux)
		return;

	for (i = 0; mod_pin_mux[i].reg_offset != -1; i++)
		MUX_CFG(mod_pin_mux[i].val, mod_pin_mux[i].reg_offset);
}

void enable_uart0_pin_mux(void)
{
	configure_module_pin_mux(uart0_pin_mux);
}


void enable_i2c0_pin_mux(void)
{
	configure_module_pin_mux(i2c0_pin_mux);
}

/*
 * The AM335x GP EVM, if daughter card(s) are connected, can have 8
 * different profiles.  These profiles determine what peripherals are
 * valid and need pinmux to be configured.
 */
#define PROFILE_NONE	0x0
#define PROFILE_0	(1 << 0)
#define PROFILE_1	(1 << 1)
#define PROFILE_2	(1 << 2)
#define PROFILE_3	(1 << 3)
#define PROFILE_4	(1 << 4)
#define PROFILE_5	(1 << 5)
#define PROFILE_6	(1 << 6)
#define PROFILE_7	(1 << 7)
#define PROFILE_MASK	0x7
#define PROFILE_ALL	0xFF

/* CPLD registers */
#define I2C_CPLD_ADDR	0x35
#define CFG_REG		0x10

static unsigned short detect_daughter_board_profile(void)
{
	unsigned short val;

	if (i2c_probe(I2C_CPLD_ADDR))
		return PROFILE_NONE;

	if (i2c_read(I2C_CPLD_ADDR, CFG_REG, 1, (unsigned char *)(&val), 2))
		return PROFILE_NONE;

	return (1 << (val & PROFILE_MASK));
}

void enable_board_pin_mux(struct am335x_baseboard_id *header)
{
	/* Do board-specific muxes. */
	if (!strncmp(header->name, "A335BONE", HDR_NAME_LEN)) {
		/* Beaglebone pinmux */
		configure_module_pin_mux(i2c1_pin_mux);
		configure_module_pin_mux(mii1_pin_mux);
		configure_module_pin_mux(mmc0_pin_mux);
		configure_module_pin_mux(mmc1_pin_mux);
	} else if (!strncmp(header->config, "SKU#01", 6)) {
		/* General Purpose EVM */
		unsigned short profile = detect_daughter_board_profile();
		configure_module_pin_mux(rgmii1_pin_mux);
		configure_module_pin_mux(mmc0_pin_mux);
		/* In profile #2 i2c1 and spi0 conflict. */
		if (profile & ~PROFILE_2)
			configure_module_pin_mux(i2c1_pin_mux);
		else if (profile == PROFILE_2) {
			configure_module_pin_mux(mmc1_pin_mux);
			configure_module_pin_mux(spi0_pin_mux);
		}
	} else if (!strncmp(header->name, "A335X_SK", HDR_NAME_LEN)) {
		/* Starter Kit EVM */
		configure_module_pin_mux(i2c1_pin_mux);
		configure_module_pin_mux(gpio0_7_pin_mux);
		configure_module_pin_mux(rgmii1_pin_mux);
		configure_module_pin_mux(mmc0_pin_mux_sk_evm);
	} else if (!strncmp(header->name, "PEPPER", HDR_NAME_LEN)) {
		/* Gumstix Pepper */
		printf("INIT PEPPER PINMUX\n");
		configure_module_pin_mux(i2c1_pin_mux_pepper);
		configure_module_pin_mux(rgmii1_pin_mux);
		configure_module_pin_mux(mmc0_pin_mux_sk_evm);
		configure_module_pin_mux(mmc1_pin_mux);
		configure_module_pin_mux(mmc1hi_pin_mux);
		configure_module_pin_mux(mmc2_pin_mux);
		configure_module_pin_mux(asp0_pin_mux);
		configure_module_pin_mux(lcd_pin_mux);
		configure_module_pin_mux(pepper_pin_mux);
	} else {
		puts("Unknown board, cannot configure pinmux.");
		hang();
	}
}
