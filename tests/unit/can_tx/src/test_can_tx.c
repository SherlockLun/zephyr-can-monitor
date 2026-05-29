#include <zephyr/ztest.h>
#include <zephyr/drivers/can.h>
#include <string.h>

#include "can_tx.h"

/* 4× the TX period (prj.conf sets all periods to 50 ms) */
#define FRAME_WAIT_MS 200

K_MSGQ_DEFINE(q_tx1, sizeof(struct can_frame), 8, 4);
K_MSGQ_DEFINE(q_tx2, sizeof(struct can_frame), 8, 4);
K_MSGQ_DEFINE(q_tx3, sizeof(struct can_frame), 8, 4);

static const struct can_filter flt_tx1 = {
	.id   = CONFIG_CAN_TX1_ID,
	.mask = CAN_STD_ID_MASK,
};
static const struct can_filter flt_tx2 = {
	.id   = CONFIG_CAN_TX2_ID,
	.mask = CAN_STD_ID_MASK,
};
static const struct can_filter flt_tx3 = {
	.id   = CONFIG_CAN_TX3_ID,
	.mask = CAN_STD_ID_MASK,
};

static void *suite_setup(void)
{
	const struct device *can_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_canbus));

	zassert_true(device_is_ready(can_dev), "CAN device not ready");
	zassert_equal(can_set_mode(can_dev, CAN_MODE_LOOPBACK), 0,
		      "can_set_mode failed");
	zassert_equal(can_start(can_dev), 0, "can_start failed");

	zassert_true(can_add_rx_filter_msgq(can_dev, &q_tx1, &flt_tx1) >= 0,
		     "TX1 filter add failed");
	zassert_true(can_add_rx_filter_msgq(can_dev, &q_tx2, &flt_tx2) >= 0,
		     "TX2 filter add failed");
	zassert_true(can_add_rx_filter_msgq(can_dev, &q_tx3, &flt_tx3) >= 0,
		     "TX3 filter add failed");

	can_tx_init(can_dev);
	return NULL;
}

static void before_each(void *unused)
{
	ARG_UNUSED(unused);
	struct can_frame tmp;

	while (k_msgq_get(&q_tx1, &tmp, K_NO_WAIT) == 0) {}
	while (k_msgq_get(&q_tx2, &tmp, K_NO_WAIT) == 0) {}
	while (k_msgq_get(&q_tx3, &tmp, K_NO_WAIT) == 0) {}
}

ZTEST_SUITE(can_tx, NULL, suite_setup, before_each, NULL, NULL);

ZTEST(can_tx, test_tx1_sends_frame_with_correct_id)
{
	struct can_frame frame;
	int ret = k_msgq_get(&q_tx1, &frame, K_MSEC(FRAME_WAIT_MS));

	zassert_equal(ret, 0, "TX1: no frame within %d ms", FRAME_WAIT_MS);
	zassert_equal(frame.id, (uint32_t)CONFIG_CAN_TX1_ID,
		      "TX1: wrong ID");
}

ZTEST(can_tx, test_tx2_sends_frame_with_correct_id)
{
	struct can_frame frame;
	int ret = k_msgq_get(&q_tx2, &frame, K_MSEC(FRAME_WAIT_MS));

	zassert_equal(ret, 0, "TX2: no frame within %d ms", FRAME_WAIT_MS);
	zassert_equal(frame.id, (uint32_t)CONFIG_CAN_TX2_ID,
		      "TX2: wrong ID");
}

ZTEST(can_tx, test_tx3_sends_frame_with_correct_id)
{
	struct can_frame frame;
	int ret = k_msgq_get(&q_tx3, &frame, K_MSEC(FRAME_WAIT_MS));

	zassert_equal(ret, 0, "TX3: no frame within %d ms", FRAME_WAIT_MS);
	zassert_equal(frame.id, (uint32_t)CONFIG_CAN_TX3_ID,
		      "TX3: wrong ID");
}

ZTEST(can_tx, test_frame_dlc_is_8)
{
	struct can_frame frame;

	zassert_equal(k_msgq_get(&q_tx1, &frame, K_MSEC(FRAME_WAIT_MS)), 0,
		      "No frame within %d ms", FRAME_WAIT_MS);
	zassert_equal(frame.dlc, 8U, "Expected DLC=8, got %d", frame.dlc);
}

ZTEST(can_tx, test_payload_changes_between_frames)
{
	struct can_frame f1, f2;

	zassert_equal(k_msgq_get(&q_tx1, &f1, K_MSEC(FRAME_WAIT_MS)), 0,
		      "First frame not received within %d ms", FRAME_WAIT_MS);
	zassert_equal(k_msgq_get(&q_tx1, &f2, K_MSEC(FRAME_WAIT_MS)), 0,
		      "Second frame not received within %d ms", FRAME_WAIT_MS);
	zassert_not_equal(memcmp(f1.data, f2.data, 8), 0,
			  "Consecutive TX1 payloads identical — RNG may be broken");
}
