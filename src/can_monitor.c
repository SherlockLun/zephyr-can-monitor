#include "can_monitor.h"
#include "logger.h"

#include <zephyr/kernel.h>
#include <zephyr/drivers/can.h>

static logging_state_t g_state;

static uint32_t g_hello_count;
static uint32_t g_log_count;

K_MSGQ_DEFINE(rx_msgq, sizeof(struct can_frame), 32, 4);

static const struct can_filter pass_all_filter = {
	.id   = 0U,
	.mask = 0U,
	.flags = 0U,
};


static inline logging_state_t initial_state(void)
{
	return IS_ENABLED(CONFIG_CAN_ID_START_ENABLED)
		? LOGGING_IDLE
		: LOGGING_ACTIVE;
}

void can_monitor_dispatch(const struct can_frame *frame)
{
#if IS_ENABLED(CONFIG_CAN_ID_HELLO_ENABLED)
	if (frame->id == CONFIG_CAN_ID_HELLO) {
		printk("hello world\n");
		g_hello_count++;
		return; /* Do not log this frame */
	}
#endif

#if IS_ENABLED(CONFIG_CAN_ID_START_ENABLED)
	if (frame->id == CONFIG_CAN_ID_START && g_state == LOGGING_IDLE) {
		g_state = LOGGING_ACTIVE;
		return;
	}
#endif

#if IS_ENABLED(CONFIG_CAN_ID_STOP_ENABLED)
	if (frame->id == CONFIG_CAN_ID_STOP && g_state == LOGGING_ACTIVE) {
		g_state = LOGGING_IDLE;
		return;
	}
#endif

	if (g_state == LOGGING_ACTIVE) {
		logger_print_frame(frame, k_uptime_get());
		g_log_count++;
	}
}

void can_monitor_init(const struct device *can_dev)
{
	g_state      = initial_state();
	g_hello_count = 0U;
	g_log_count   = 0U;

	int filter_id = can_add_rx_filter_msgq(can_dev, &rx_msgq,
					       &pass_all_filter);
	if (filter_id < 0) {
		printk("can_monitor: failed to add RX filter (%d)\n",
		       filter_id);
	}
}

void can_monitor_reset(void)
{
	g_state       = initial_state();
	g_hello_count = 0U;
	g_log_count   = 0U;
}

logging_state_t can_monitor_get_state(void)
{
	return g_state;
}

uint32_t can_monitor_get_hello_count(void)
{
	return g_hello_count;
}

uint32_t can_monitor_get_log_count(void)
{
	return g_log_count;
}

static void monitor_thread_entry(void *arg1, void *arg2, void *arg3)
{
	ARG_UNUSED(arg1);
	ARG_UNUSED(arg2);
	ARG_UNUSED(arg3);

	struct can_frame frame;

	while (1) {
		/* Block until a frame is available */
		k_msgq_get(&rx_msgq, &frame, K_FOREVER);
		can_monitor_dispatch(&frame);
	}
}

K_THREAD_DEFINE(monitor_tid,
		2048,
		monitor_thread_entry,
		NULL, NULL, NULL,
		5,                  /* priority (preemptible)        */
		0,                  /* options                       */
		0);                 /* start delay (ms) — 0 = now    */
