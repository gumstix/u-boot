/*
 * Copyright (C) 2015 Gumstix, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#define DEV_ATTR_MAX_OFFSET 5
#define DEV_ATTR_MIN_OFFSET 0

void enable_i2c_pin_mux(void);
void enable_board_pin_mux(void);
void enable_uart0_pin_mux(void);
#endif
