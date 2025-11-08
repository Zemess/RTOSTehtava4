#include "uartDef.h"
#include "dispatcher.h"
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <string.h>
#include "debug.h"

#define UART_DEVICE_NODE DT_CHOSEN(zephyr_shell_uart)
static const struct device *const uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

static void uart_cb(const struct device *dev, void *user_data) {
    uint8_t c;
    if (uart_irq_update(dev) && uart_irq_rx_ready(dev)) {
        while (uart_fifo_read(dev, &c, 1) == 1) {
            dispatcher_event_t evt = { .type = EVENT_LED_CMD };
            evt.msg[0] = c;
            evt.msg[1] = '\0';
            dispatcher_post(&evt);
        }
    }
}

void uartDef_init(void) {
    if (!device_is_ready(uart_dev)) {
        DBG("Error: UART device not ready\n");
        return;
    }
    uart_irq_callback_user_data_set(uart_dev, uart_cb, NULL);
    uart_irq_rx_enable(uart_dev);
    DBG("UART initialized with RX interrupt\n");
}
