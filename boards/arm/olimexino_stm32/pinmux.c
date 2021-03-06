/*
 * Copyright (c) 2016 Open-RnD Sp. z o.o.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <kernel.h>
#include <device.h>
#include <init.h>
#include <drivers/pinmux.h>
#include <sys/sys_io.h>
#include <soc.h>

#include <pinmux/stm32/pinmux_stm32.h>

/* pin assignments for OLIMEXINO-STM32 board */
static const struct pin_config pinconf[] = {
#if DT_HAS_NODE(DT_NODELABEL(usart1))
	{STM32_PIN_PA9,  STM32F1_PINMUX_FUNC_PA9_USART1_TX},
	{STM32_PIN_PA10, STM32F1_PINMUX_FUNC_PA10_USART1_RX},
#endif
#if DT_HAS_NODE(DT_NODELABEL(usart2))
	{STM32_PIN_PA2, STM32F1_PINMUX_FUNC_PA2_USART2_TX},
	{STM32_PIN_PA3, STM32F1_PINMUX_FUNC_PA3_USART2_RX},
#endif
#if DT_HAS_NODE(DT_NODELABEL(usart3))
	{STM32_PIN_PB10, STM32F1_PINMUX_FUNC_PB10_USART3_TX},
	{STM32_PIN_PB11, STM32F1_PINMUX_FUNC_PB11_USART3_RX},
#endif
#if DT_HAS_NODE(DT_NODELABEL(i2c1))
	{STM32_PIN_PB6, STM32F1_PINMUX_FUNC_PB6_I2C1_SCL},
	{STM32_PIN_PB7, STM32F1_PINMUX_FUNC_PB7_I2C1_SDA},
#endif
#if DT_HAS_NODE(DT_NODELABEL(i2c2))
	{STM32_PIN_PB10, STM32F1_PINMUX_FUNC_PB10_I2C2_SCL},
	{STM32_PIN_PB11, STM32F1_PINMUX_FUNC_PB11_I2C2_SDA},
#endif
#if DT_HAS_NODE(DT_NODELABEL(spi1))
#ifdef CONFIG_SPI_STM32_USE_HW_SS
	{STM32_PIN_PA4, STM32F1_PINMUX_FUNC_PA4_SPI1_MASTER_NSS_OE},
#endif /* CONFIG_SPI_STM32_USE_HW_SS */
	{STM32_PIN_PA5, STM32F1_PINMUX_FUNC_PA5_SPI1_MASTER_SCK},
	{STM32_PIN_PA6, STM32F1_PINMUX_FUNC_PA6_SPI1_MASTER_MISO},
	{STM32_PIN_PA7, STM32F1_PINMUX_FUNC_PA7_SPI1_MASTER_MOSI},
#endif
#if DT_HAS_NODE(DT_NODELABEL(spi2))
#ifdef CONFIG_SPI_STM32_USE_HW_SS
	{STM32_PIN_PB12, STM32F1_PINMUX_FUNC_PB12_SPI2_MASTER_NSS_OE},
#endif /* CONFIG_SPI_STM32_USE_HW_SS */
	{STM32_PIN_PB13, STM32F1_PINMUX_FUNC_PB13_SPI2_MASTER_SCK},
	{STM32_PIN_PB14, STM32F1_PINMUX_FUNC_PB14_SPI2_MASTER_MISO},
	{STM32_PIN_PB15, STM32F1_PINMUX_FUNC_PB15_SPI2_MASTER_MOSI},
#endif
#if DT_HAS_NODE(DT_NODELABEL(pwm1))
	{STM32_PIN_PA8, STM32F1_PINMUX_FUNC_PA8_PWM1_CH1},
#endif
#ifdef CONFIG_USB_DC_STM32
	{STM32_PIN_PA11, STM32F1_PINMUX_FUNC_PA11_USB_DM},
	{STM32_PIN_PA12, STM32F1_PINMUX_FUNC_PA12_USB_DP},
#endif /* CONFIG_USB_DC_STM32 */
#if DT_HAS_NODE(DT_NODELABEL(can1))
	{STM32_PIN_PB8, STM32F1_PINMUX_FUNC_PB8_CAN_RX},
	{STM32_PIN_PB9, STM32F1_PINMUX_FUNC_PB9_CAN_TX},
#endif
};

static int pinmux_stm32_init(struct device *port)
{
	ARG_UNUSED(port);

	stm32_setup_pins(pinconf, ARRAY_SIZE(pinconf));
#if DT_HAS_NODE(DT_NODELABEL(can1))
	/* Set pin-mux so that CAN1 is on PB8 and PB9 */
	AFIO->MAPR |= AFIO_MAPR_CAN_REMAP_REMAP2;
#endif

	return 0;
}

SYS_INIT(pinmux_stm32_init, PRE_KERNEL_1, CONFIG_PINMUX_STM32_DEVICE_INITIALIZATION_PRIORITY);
