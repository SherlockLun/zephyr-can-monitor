#ifndef LOGGER_H
#define LOGGER_H

#include <stddef.h>
#include <stdint.h>
#include <zephyr/drivers/can.h>

int logger_format_frame(char *buf, size_t buf_len,
			const struct can_frame *frame, int64_t ts_ms);

void logger_print_frame(const struct can_frame *frame, int64_t ts_ms);

#endif
