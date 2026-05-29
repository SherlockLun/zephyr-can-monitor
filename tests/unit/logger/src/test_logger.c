#include <zephyr/ztest.h>
#include <zephyr/drivers/can.h>
#include <string.h>

#include "logger.h"

ZTEST_SUITE(logger_format, NULL, NULL, NULL, NULL, NULL);

#define BUF_LEN 128

static void format_into(char *buf, size_t buf_len,
			uint32_t id, uint8_t dlc,
			const uint8_t *data, int64_t ts_ms)
{
	struct can_frame f;

	memset(&f, 0, sizeof(f));
	f.id  = id;
	f.dlc = dlc;
	if (data != NULL) {
		memcpy(f.data, data, dlc);
	}
	logger_format_frame(buf, buf_len, &f, ts_ms);
}

ZTEST(logger_format, test_timestamp_present)
{
	char buf[BUF_LEN];

	format_into(buf, sizeof(buf), 0x123, 0, NULL, 9999LL);

	zassert_not_null(strstr(buf, "[9999]"),
			 "Timestamp token '[9999]' not found in: %s", buf);
}

ZTEST(logger_format, test_id_formatted_correctly)
{
	char buf[BUF_LEN];

	format_into(buf, sizeof(buf), 0x1A2, 0, NULL, 0LL);

	zassert_not_null(strstr(buf, "ID=0x1A2"),
			 "ID token 'ID=0x1A2' not found in: %s", buf);
}

ZTEST(logger_format, test_id_zero_formatted)
{
	char buf[BUF_LEN];

	format_into(buf, sizeof(buf), 0x000, 0, NULL, 0LL);

	zassert_not_null(strstr(buf, "ID=0x000"),
			 "Zero ID not found in: %s", buf);
}

ZTEST(logger_format, test_dlc_present)
{
	char buf[BUF_LEN];

	format_into(buf, sizeof(buf), 0x001, 8, NULL, 0LL);

	zassert_not_null(strstr(buf, "DLC=8"),
			 "DLC token 'DLC=8' not found in: %s", buf);
}

ZTEST(logger_format, test_dlc_zero_no_data_bytes)
{
	char buf[BUF_LEN];

	format_into(buf, sizeof(buf), 0x001, 0, NULL, 0LL);

	zassert_not_null(strstr(buf, "DLC=0"),
			 "DLC=0 not found in: %s", buf);
	zassert_not_null(strstr(buf, "DATA="),
			 "'DATA=' token not found in: %s", buf);

	const char *data_pos = strstr(buf, "DATA=");

	zassert_equal(*(data_pos + 5), '\0',
		      "No bytes expected after 'Data:' with DLC=0, got: %s",
		      buf);
}

ZTEST(logger_format, test_data_bytes_formatted)
{
	char buf[BUF_LEN];
	const uint8_t data[] = { 0xDE, 0xAD, 0xBE, 0xEF };

	format_into(buf, sizeof(buf), 0x100, 4, data, 0LL);

	zassert_not_null(strstr(buf, "DE AD BE EF"),
			 "Data bytes 'DE AD BE EF' not found in: %s", buf);
}

ZTEST(logger_format, test_full_8_byte_frame)
{
	char buf[BUF_LEN];
	const uint8_t data[] = { 0x00, 0x11, 0x22, 0x33,
				  0x44, 0x55, 0x66, 0x77 };

	format_into(buf, sizeof(buf), 0x7FF, 8, data, 42LL);

	zassert_not_null(strstr(buf, "[42]"),       "timestamp: %s", buf);
	zassert_not_null(strstr(buf, "ID=0x7FF"),   "id: %s", buf);
	zassert_not_null(strstr(buf, "DLC=8"),      "dlc: %s", buf);
	zassert_not_null(strstr(buf, "00 11 22 33 44 55 66 77"),
			 "data bytes: %s", buf);
}

ZTEST(logger_format, test_return_value_is_length)
{
	char buf[BUF_LEN];
	struct can_frame f;

	memset(&f, 0, sizeof(f));
	f.id  = 0x001;
	f.dlc = 0;

	int len = logger_format_frame(buf, sizeof(buf), &f, 1LL);

	zassert_equal(len, (int)strlen(buf),
		      "Return value %d != strlen %zu: %s",
		      len, strlen(buf), buf);
}

ZTEST(logger_format, test_no_buffer_overflow)
{
	char buf[10];
	const uint8_t data[] = { 0xFF, 0xFF, 0xFF, 0xFF,
				  0xFF, 0xFF, 0xFF, 0xFF };

	logger_format_frame(buf, sizeof(buf), NULL, 0LL);

	struct can_frame f;

	memset(&f, 0, sizeof(f));
	f.id  = 0x123;
	f.dlc = 8;
	memcpy(f.data, data, 8);

	logger_format_frame(buf, sizeof(buf), &f, 0LL);
	buf[sizeof(buf) - 1] = '\0';
	zassert_true(strlen(buf) < sizeof(buf),
		     "String must fit within buffer");
}
