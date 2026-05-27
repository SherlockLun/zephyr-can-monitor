#include "logger.h"

#include <stdio.h>
#include <zephyr/kernel.h>

#define LOGGER_BUF_LEN 72

int logger_format_frame(char *buf, size_t buf_len,
			const struct can_frame *frame, int64_t ts_ms)
{
	int pos = 0;
	int ret;

	if (frame == NULL || buf == NULL || buf_len == 0) {
		if (buf != NULL && buf_len > 0) {
			buf[0] = '\0';
		}
		return 0;
	}

	/* Timestamp and header */
	ret = snprintf(buf + pos, buf_len - pos,
		       "[%lld] ID:0x%03X DLC:%d Data:",
		       (long long)ts_ms, frame->id, frame->dlc);
	if (ret < 0 || (size_t)ret >= buf_len - pos) {
		return ret;
	}
	pos += ret;

	/* Data bytes */
	for (uint8_t i = 0; i < frame->dlc; i++) {
		const char *sep = (i == 0) ? "" : " ";

		ret = snprintf(buf + pos, buf_len - pos,
			       "%s%02X", sep, frame->data[i]);
		if (ret < 0 || (size_t)ret >= buf_len - pos) {
			break;
		}
		pos += ret;
	}

	return pos;
}

void logger_print_frame(const struct can_frame *frame, int64_t ts_ms)
{
	char buf[LOGGER_BUF_LEN];

	logger_format_frame(buf, sizeof(buf), frame, ts_ms);
	printk("%s\n", buf);
}
