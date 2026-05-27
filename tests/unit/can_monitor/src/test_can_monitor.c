#include <zephyr/ztest.h>
#include <zephyr/drivers/can.h>
#include <string.h>

#include "can_monitor.h"

struct monitor_fixture {
	const struct device *can_dev;
};

static void *suite_setup(void)
{
	static struct monitor_fixture f;

	f.can_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_canbus));
	zassert_true(device_is_ready(f.can_dev),
		     "Fake CAN device not ready");

	return &f;
}

static void before_each(void *data)
{
	ARG_UNUSED(data);
	struct monitor_fixture *f = (struct monitor_fixture *)data;

	can_monitor_init(f->can_dev);
}

ZTEST_SUITE(can_monitor_default, NULL, suite_setup, before_each, NULL, NULL);

static struct can_frame make_frame(uint32_t id)
{
	struct can_frame f;

	memset(&f, 0, sizeof(f));
	f.id  = id;
	f.dlc = 2;
	f.data[0] = 0xAB;
	f.data[1] = 0xCD;
	return f;
}

ZTEST(can_monitor_default, test_initial_state_is_active)
{
	zassert_equal(can_monitor_get_state(), LOGGING_ACTIVE,
		      "Expected LOGGING_ACTIVE on startup (no start ID)");
}

ZTEST(can_monitor_default, test_frame_logged_when_active)
{
	zassert_equal(can_monitor_get_state(), LOGGING_ACTIVE, "pre-cond");

	struct can_frame f = make_frame(0x042);
	can_monitor_dispatch(&f);

	zassert_equal(can_monitor_get_log_count(), 1U,
		      "Frame should have been logged");
}

ZTEST(can_monitor_default, test_multiple_frames_logged)
{
	struct can_frame f = make_frame(0x010);

	can_monitor_dispatch(&f);
	can_monitor_dispatch(&f);
	can_monitor_dispatch(&f);

	zassert_equal(can_monitor_get_log_count(), 3U,
		      "All three frames should be logged");
}

ZTEST(can_monitor_default, test_log_count_zero_after_reset)
{
	struct can_frame f = make_frame(0x001);

	can_monitor_dispatch(&f);
	zassert_equal(can_monitor_get_log_count(), 1U, "one frame logged");

	can_monitor_reset();
	zassert_equal(can_monitor_get_log_count(), 0U,
		      "log_count reset to 0");
}

ZTEST(can_monitor_default, test_hello_count_initially_zero)
{
	zassert_equal(can_monitor_get_hello_count(), 0U,
		      "hello_count should be 0 at init");
}

ZTEST(can_monitor_default, test_no_hello_without_hello_id)
{
	struct can_frame f = make_frame(0x202);

	can_monitor_dispatch(&f);

	zassert_equal(can_monitor_get_hello_count(), 0U,
		      "hello_count must stay 0 when HELLO_ID disabled");
	zassert_equal(can_monitor_get_log_count(), 1U,
		      "Frame logged normally (not treated as HELLO)");
}
