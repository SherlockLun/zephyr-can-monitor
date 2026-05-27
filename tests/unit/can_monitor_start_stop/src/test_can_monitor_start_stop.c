#include <zephyr/ztest.h>
#include <zephyr/drivers/can.h>
#include <string.h>

#include "can_monitor.h"

struct monitor_ss_fixture {
	const struct device *can_dev;
};

static void *suite_setup(void)
{
	static struct monitor_ss_fixture f;

	f.can_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_canbus));
	zassert_true(device_is_ready(f.can_dev), "Fake CAN not ready");
	return &f;
}

static void before_each(void *data)
{
	struct monitor_ss_fixture *f = (struct monitor_ss_fixture *)data;

	can_monitor_init(f->can_dev);
}

ZTEST_SUITE(can_monitor_start_stop, NULL, suite_setup, before_each, NULL, NULL);

static struct can_frame make_frame(uint32_t id)
{
	struct can_frame f;

	memset(&f, 0, sizeof(f));
	f.id  = id;
	f.dlc = 1;
	f.data[0] = 0xFF;
	return f;
}

ZTEST(can_monitor_start_stop, test_initial_state_is_idle)
{
	zassert_equal(can_monitor_get_state(), LOGGING_IDLE,
		      "Expected LOGGING_IDLE when START_ID is configured");
}

ZTEST(can_monitor_start_stop, test_start_id_activates_logging)
{
	zassert_equal(can_monitor_get_state(), LOGGING_IDLE, "pre-cond");

	struct can_frame start = make_frame(CONFIG_CAN_ID_START);

	can_monitor_dispatch(&start);

	zassert_equal(can_monitor_get_state(), LOGGING_ACTIVE,
		      "START ID must transition to LOGGING_ACTIVE");
}

ZTEST(can_monitor_start_stop, test_start_frame_not_logged)
{
	struct can_frame start = make_frame(CONFIG_CAN_ID_START);

	can_monitor_dispatch(&start);

	zassert_equal(can_monitor_get_log_count(), 0U,
		      "START control frame must not be counted as logged");
}

ZTEST(can_monitor_start_stop, test_stop_id_deactivates_logging)
{
	struct can_frame start = make_frame(CONFIG_CAN_ID_START);
	struct can_frame stop  = make_frame(CONFIG_CAN_ID_STOP);

	can_monitor_dispatch(&start);
	zassert_equal(can_monitor_get_state(), LOGGING_ACTIVE, "pre-cond");

	can_monitor_dispatch(&stop);
	zassert_equal(can_monitor_get_state(), LOGGING_IDLE,
		      "STOP ID must transition to LOGGING_IDLE");
}

ZTEST(can_monitor_start_stop, test_frames_not_logged_when_idle)
{
	struct can_frame f = make_frame(0x042);

	can_monitor_dispatch(&f);
	can_monitor_dispatch(&f);

	zassert_equal(can_monitor_get_log_count(), 0U,
		      "No frames should be logged while in IDLE state");
}

ZTEST(can_monitor_start_stop, test_frames_logged_when_active)
{
	struct can_frame start = make_frame(CONFIG_CAN_ID_START);
	struct can_frame data  = make_frame(0x042);

	can_monitor_dispatch(&start);
	can_monitor_dispatch(&data);
	can_monitor_dispatch(&data);

	zassert_equal(can_monitor_get_log_count(), 2U,
		      "Both data frames should be logged");
}

ZTEST(can_monitor_start_stop, test_hello_id_from_idle)
{
	struct can_frame hello = make_frame(CONFIG_CAN_ID_HELLO);

	can_monitor_dispatch(&hello);

	zassert_equal(can_monitor_get_hello_count(), 1U,
		      "hello_count should be 1 after HELLO frame");
	zassert_equal(can_monitor_get_log_count(), 0U,
		      "HELLO frame must not be counted as logged");
}

ZTEST(can_monitor_start_stop, test_hello_id_from_active)
{
	struct can_frame start = make_frame(CONFIG_CAN_ID_START);
	struct can_frame hello = make_frame(CONFIG_CAN_ID_HELLO);

	can_monitor_dispatch(&start);
	can_monitor_dispatch(&hello);

	zassert_equal(can_monitor_get_hello_count(), 1U,
		      "HELLO must trigger in ACTIVE state too");
	zassert_equal(can_monitor_get_state(), LOGGING_ACTIVE,
		      "State must remain ACTIVE after HELLO");
}

ZTEST(can_monitor_start_stop, test_full_start_stop_cycle)
{
	struct can_frame start = make_frame(CONFIG_CAN_ID_START);
	struct can_frame stop  = make_frame(CONFIG_CAN_ID_STOP);
	struct can_frame data  = make_frame(0x099);

	can_monitor_dispatch(&start);
	zassert_equal(can_monitor_get_state(), LOGGING_ACTIVE, "cycle1 ACTIVE");
	can_monitor_dispatch(&data);
	can_monitor_dispatch(&stop);
	zassert_equal(can_monitor_get_state(), LOGGING_IDLE, "cycle1 IDLE");

	can_monitor_dispatch(&start);
	zassert_equal(can_monitor_get_state(), LOGGING_ACTIVE, "cycle2 ACTIVE");
	can_monitor_dispatch(&data);
	can_monitor_dispatch(&data);

	zassert_equal(can_monitor_get_log_count(), 3U,
		      "3 data frames total across both cycles");
}
