#include "can_tx.h"

#include <zephyr/kernel.h>
#include <zephyr/drivers/can.h>
#include <zephyr/random/random.h>

static const struct device *g_can_dev;

#define TX_STACK_SIZE 1024

static struct k_thread tx_thread_data[3];

K_THREAD_STACK_ARRAY_DEFINE(tx_stacks, 3, TX_STACK_SIZE);

struct tx_cfg {
	uint32_t id;
	uint32_t period_ms;
};

static const struct tx_cfg tx_cfgs[3] = {
	{ CONFIG_CAN_TX1_ID, CONFIG_CAN_TX1_PERIOD_MS },
	{ CONFIG_CAN_TX2_ID, CONFIG_CAN_TX2_PERIOD_MS },
	{ CONFIG_CAN_TX3_ID, CONFIG_CAN_TX3_PERIOD_MS },
};

static void tx_send_random(uint32_t can_id, uint32_t period_ms)
{
	struct can_frame frame = {
		.id    = can_id,
		.dlc   = 8,
		.flags = 0,
	};

	sys_rand_get(frame.data, sizeof(frame.data));

	int err = can_send(g_can_dev, &frame, K_MSEC(10), NULL, NULL);

	if (err != 0) {
		printk("can_tx: send 0x%03X failed (%d)\n", can_id, err);
	}

	k_sleep(K_MSEC(period_ms));
}

static void tx_entry(void *p1, void *p2, void *p3)
{
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	const struct tx_cfg *cfg = p1;

	while (1) {
		tx_send_random(cfg->id, cfg->period_ms);
	}
}

void can_tx_init(const struct device *can_dev)
{
	g_can_dev = can_dev;

	for (int i = 0; i < 3; i++) {
		k_thread_create(&tx_thread_data[i], tx_stacks[i],
				TX_STACK_SIZE,
				tx_entry, (void *)&tx_cfgs[i], NULL, NULL,
				7, 0, K_NO_WAIT);
		k_thread_name_set(&tx_thread_data[i], i == 0 ? "can_tx1"
							      : i == 1 ? "can_tx2"
								       : "can_tx3");
	}
}
