/**
 * @file 	bluetooth.c
 *
 * @brief 	Library to interact with the HC-05 bluetooth module
 *
 * @date 	July, 2020
 *
 * @author 	James Zhang (jameszha@andrew.cmu.edu)
 */

#include "bluetooth.h"
#include <kernel.h>
#include <zephyr.h>
#include <sys/printk.h>

/** @brief 	UART 6 corresponds to the standard Arduino RX and TX (i.e. D0 and D1, respectively) */
#define BLUETOOTH_UART_DEVICE_NAME "UART_6"

/** @brief  UART Configuration */
const struct uart_config bluetooth_hc05_cfg = {
		.baudrate = 9600,
		.parity = UART_CFG_PARITY_NONE,
		.stop_bits = UART_CFG_STOP_BITS_1,
		.data_bits = UART_CFG_DATA_BITS_8,
		.flow_ctrl = UART_CFG_FLOW_CTRL_NONE
	};

K_MSGQ_DEFINE(msg_queue, sizeof(char), 1024, 4);

/**
 * @brief Bluetooth interrupt handler to process bytes received
 * 
 * Receives 1 byte upon each RX interrupt and places it in the message queue for the consumer
 * to process.
 *
 * @param dev The UART device (obtained by a call to device_get_binding())
 */
static void bluetooth_irq_handler(struct device *dev)
{
	if (!uart_irq_update(dev)) {
		printk("Bluetooth IRQ hander: something went wrong.");
		return;
	}

	char data;
	// If data is ready, read a byte and enqueue it for the consumer to read later
	// If the buffer is full, purge old messages in favor of new ones
	if (uart_irq_rx_ready(dev)) {
		uart_fifo_read(dev, &data, 1);
		while (k_msgq_put(&msg_queue, &data, K_NO_WAIT) != 0) {
            k_msgq_purge(&msg_queue);
        }
	}
	return;
}


/**
 * @brief Initializes Bluetooth and enables UART interrupts
 */
int bluetooth_init(void)
{
	struct device *uart_dev = device_get_binding(BLUETOOTH_UART_DEVICE_NAME);

	if (!uart_dev) {
		printk("Cannot get UART device\n");
		return -1;
	}

	if (uart_configure(uart_dev, &bluetooth_hc05_cfg)) {
		printk("Failed to configure Bluetooth UART\n");
		return -1;
	}

	uart_irq_callback_set(uart_dev, bluetooth_irq_handler);
	uart_irq_rx_enable(uart_dev);

	return 0;
}

/**
 * @brief Get a byte via interrupt-driven UART
 * 
 * @return Next byte from the UART message queue 
 */
char bluetooth_get_byte(void)
{
	char data;

	k_msgq_get(&msg_queue, &data, K_FOREVER);
	
	return data;
}

/**
 * @brief Get a byte via polling
 * 
 * @return Byte polled from the UART device
 */
char bluetooth_polling_get_byte(void)
{
	struct device *uart_dev = device_get_binding(BLUETOOTH_UART_DEVICE_NAME);

	if (!uart_dev) {
		printk("Cannot get UART device\n");
		return -1;
	}
	
	char data;
	while(uart_poll_in(uart_dev, &data) < 0);
	
	return data;
}