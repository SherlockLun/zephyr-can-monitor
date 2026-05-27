#ifndef CAN_MONITOR_H
#define CAN_MONITOR_H

#include <zephyr/drivers/can.h>

typedef enum {
	LOGGING_IDLE,
	LOGGING_ACTIVE,
} logging_state_t;

void can_monitor_init(const struct device *can_dev);

logging_state_t can_monitor_get_state(void);

void can_monitor_reset(void);
void can_monitor_dispatch(const struct can_frame *frame);

uint32_t can_monitor_get_hello_count(void);
uint32_t can_monitor_get_log_count(void);

#endif
