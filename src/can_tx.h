#ifndef CAN_TX_H
#define CAN_TX_H

#include <zephyr/drivers/can.h>

void can_tx_init(const struct device *can_dev);

#endif
