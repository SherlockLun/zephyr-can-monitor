#include <zephyr/kernel.h>
#include <zephyr/drivers/can.h>
#include <zephyr/devicetree.h>

#include "can_monitor.h"
#include "can_tx.h"

int main(void)
{
	int err;

	/* ------------------------------------------------------------------
	 * 1. Get the CAN device from the device tree
	 * ------------------------------------------------------------------ */
	const struct device *can_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_canbus));

	if (!device_is_ready(can_dev)) {
		printk("CAN device not ready — halting\n");
		return -ENODEV;
	}

	/* ------------------------------------------------------------------
	 * 2. Configure CAN mode and start the controller
	 * ------------------------------------------------------------------ */
	can_mode_t mode = IS_ENABLED(CONFIG_CAN_LOOPBACK_MODE)
		? CAN_MODE_LOOPBACK : CAN_MODE_NORMAL;
	err = can_set_mode(can_dev, mode);
	if (err != 0) {
		printk("can_set_mode failed (%d)\n", err);
		return err;
	}

	err = can_start(can_dev);
	if (err != 0) {
		printk("can_start failed (%d)\n", err);
		return err;
	}

	printk("CAN logger started — bitrate %d bps\n",
	       CONFIG_CAN_LOGGER_BITRATE);

#if IS_ENABLED(CONFIG_CAN_ID_START_ENABLED)
	printk("  Waiting for start ID 0x%03X\n", CONFIG_CAN_ID_START);
#else
	printk("  Logging immediately\n");
#endif

	can_monitor_init(can_dev);

	can_tx_init(can_dev);

	return 0;
}
