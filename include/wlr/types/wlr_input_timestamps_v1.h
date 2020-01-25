/*
 * This an unstable interface of wlroots. No guarantees are made regarding the
 * future consistency of this API.
 */
#ifndef WLR_USE_UNSTABLE
#error "Add -DWLR_USE_UNSTABLE to enable unstable wlroots features"
#endif

#ifndef WLR_TYPES_WLR_INPUT_TIMESTAMPS_V1_H
#define WLR_TYPES_WLR_INPUT_TIMESTAMPS_V1_H

#include <wayland-server-core.h>

struct wlr_seat;
struct timespec;

/*
 * This interface allows clients to request and receive high-resolution
 * timestamps for input events.
 */

struct wlr_input_timestamps_manager_v1 {
	struct wl_global *global;

	struct {
		struct wl_signal destroy;
	} events;

	struct wl_listener display_destroy;

	void *data;
};

struct wlr_input_timestamps_v1 {
	struct wl_listener pointer_motion;
	struct wl_listener pointer_button;
	struct wl_listener pointer_axis;
	struct wl_listener pointer_axis_stop;

	struct wl_listener keyboard_key;

	struct wl_listener touch_down;
	struct wl_listener touch_up;
	struct wl_listener touch_motion;

	struct wl_listener seat_destroy;
	struct wl_listener manager_destroy;
};

enum wlr_input_timestamps_type_v1 {
	WLR_INPUT_TIMESTAMP_KEYBOARD,
	WLR_INPUT_TIMESTAMP_POINTER,
	WLR_INPUT_TIMESTAMP_TOUCH,
};

struct wlr_input_timestamps_manager_v1 *wlr_input_timestamps_manager_v1_create(
	struct wl_display *display);

/*
 * Immediately sends the timestamp event to the client.
 * Don't use this with `wlr_seat_*_notify`, because the seat event will respect
 * grabs, and thus may not send an input event after.
 */

void wlr_input_timestamps_manager_v1_send_timestamp(
	struct wlr_input_timestamps_manager_v1 *manager, struct wlr_seat *seat,
	enum wlr_input_timestamps_type_v1 type, struct timespec *ts);

/*
 * Sets the timestamp for the next event with timestamp set on the wlr_seat.
 */
void wlr_input_timestamps_manager_v1_notify_timestamp(
	struct wlr_input_timestamps_manager_v1 *manager, struct wlr_seat *seat,
	enum wlr_input_timestamps_type_v1 type, struct timespec *ts);

#endif
