/*
 *  TP-LINK TL-WA7210N board support
 *
 *  Copyright (C) 2014 Rocco Folino <rocco.folino@tanaza.com>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/gpio.h>
#include <linux/platform_device.h>

#include <asm/mach-ath79/ath79.h>
#include <asm/mach-ath79/ar71xx_regs.h>

#include "common.h"
#include "dev-eth.h"
#include "dev-m25p80.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-wmac.h"
#include "machtypes.h"

#define TL_WA7210N_V2_GPIO_LED_LAN 17
#define TL_WA7210N_V2_GPIO_LED_SIGNAL1 0
#define TL_WA7210N_V2_GPIO_LED_SIGNAL2 1
#define TL_WA7210N_V2_GPIO_LED_SIGNAL3 27
#define TL_WA7210N_V2_GPIO_LED_SIGNAL4 26

#define TL_WA7210N_V2_GPIO_BTN_RESET 11

#define TL_WA7210N_V2_KEYS_POLL_INTERVAL 20 /* msecs */
#define TL_WA7210N_V2_KEYS_DEBOUNCE_INTERVAL (3 * TL_WA7210N_V2_KEYS_POLL_INTERVAL)

#define TL_WA7210N_V2_MAC0_OFFSET 0x0000
#define TL_WA7210N_V2_MAC1_OFFSET 0x0006
#define TL_WA7210N_V2_CALDATA_OFFSET 0x1000
#define TL_WA7210N_V2_WMAC_MAC_OFFSET 0x1002

static const char *tl_wa7210n_v2_part_probes[] = {
	"tp-link",
	NULL,
};

static struct flash_platform_data tl_wa7210n_v2_flash_data = {
	.part_probes	= tl_wa7210n_v2_part_probes,
};

static struct gpio_led tl_wa7210n_v2_leds_gpio[] __initdata = {
	{
		.name		= "tp-link:green:lan",
		.gpio		= TL_WA7210N_V2_GPIO_LED_LAN,
		.active_low	= 0,
	},
	{
		.name		= "tp-link:green:signal1",
		.gpio		= TL_WA7210N_V2_GPIO_LED_SIGNAL1,
		.active_low	= 0,
	},
	{
		.name		= "tp-link:green:signal2",
		.gpio		= TL_WA7210N_V2_GPIO_LED_SIGNAL2,
		.active_low	= 0,
	},
	{
		.name		= "tp-link:green:signal3",
		.gpio		= TL_WA7210N_V2_GPIO_LED_SIGNAL3,
		.active_low	= 1,
	},
	{
		.name		= "tp-link:green:signal4",
		.gpio		= TL_WA7210N_V2_GPIO_LED_SIGNAL4,
		.active_low	= 1,
	}
};

static struct gpio_keys_button tl_wa7210n_v2_gpio_keys[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = TL_WA7210N_V2_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= TL_WA7210N_V2_GPIO_BTN_RESET,
		.active_low	= 0,
	}
};

static void __init tl_wa7210n_v2_setup(void)
{
	u8 *art = (u8 *) KSEG1ADDR(0x1fff0000);

	ath79_register_m25p80(&tl_wa7210n_v2_flash_data);

	ath79_register_leds_gpio(-1, ARRAY_SIZE(tl_wa7210n_v2_leds_gpio),
				 tl_wa7210n_v2_leds_gpio);
	ath79_register_gpio_keys_polled(-1, TL_WA7210N_V2_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(tl_wa7210n_v2_gpio_keys),
					tl_wa7210n_v2_gpio_keys);

	ath79_register_wmac(art + TL_WA7210N_V2_CALDATA_OFFSET,
			    art + TL_WA7210N_V2_WMAC_MAC_OFFSET);

	ath79_init_mac(ath79_eth0_data.mac_addr, art + TL_WA7210N_V2_MAC0_OFFSET, 0);
	ath79_init_mac(ath79_eth1_data.mac_addr, art + TL_WA7210N_V2_MAC1_OFFSET, 0);

	ath79_register_mdio(0, 0x0);

	/* LAN ports */
	ath79_register_eth(1);
	
	/* WAN port */
	ath79_register_eth(0);
}

MIPS_MACHINE(ATH79_MACH_TL_WA7210N_V2, "TL-WA7210N-v2", "TP-LINK TL-WA7210N v2", tl_wa7210n_v2_setup);
