#include <stdlib.h>
#include <wlr/types/wlr_input_timestamps_v1.h>
#include <wlr/util/log.h>
#include "input-timestamps-unstable-v1-protocol.h"
#include "util/signal.h"

#define INPUT_TIMESTAMPS_MANAGER_VERSION 1

static void manager_handle_destroy(struct wl_client *client,
		struct wl_resource *resource) {

}

static void manager_handle_get_keyboard_timestamps(struct wl_client *client,
		struct wl_resource *resource, uint32_t id,
		struct wl_resource *keyboard) {
}

static void manager_handle_get_pointer_timestamps(struct wl_client *client,
		struct wl_resource *resource, uint32_t id,
		struct wl_resource *pointer) {
}

static void manager_handle_get_touch_timestamps(struct wl_client *client,
		struct wl_resource *resource, uint32_t id, struct wl_resource *touch) {
}

static void manager_handle_resource_destroy(struct wl_resource *wl_resource) {
}

static const struct zwp_input_timestamps_manager_v1_interface
		zwp_input_timestamps_manager_v1_implementation = {
};

static void manager_bind(struct wl_client *wl_client, void *data,
		uint32_t version, uint32_t id) {
	struct wlr_input_timestamps_manager_v1 *manager = data;
	struct wl_resource *resource = wl_resource_create(wl_client,
			&zwp_input_timestamps_manager_v1_interface, version, id);
	if (resource == NULL) {
		wl_client_post_no_memory(wl_client);
		return;
	}
	wl_resource_set_implementation(resource,
			&zwp_input_timestamps_manager_v1_implementation, manager,
			manager_handle_resource_destroy);
}

static void handle_display_destroy(struct wl_listener *listener, void *data) {
	struct wlr_input_timestamps_manager_v1 *manager =
		wl_container_of(data, manager, display_destroy);
	wlr_signal_emit_safe(&manager->events.destroy, manager);
	wl_list_remove(&manager->display_destroy.link);
	wl_global_destroy(manager->global);
	free(manager);
}

struct wlr_input_timestamps_manager_v1 *wlr_input_timestamps_manager_v1_create(
		struct wl_display *display) {
	struct wlr_input_timestamps_manager_v1 *manager =
		calloc(1, sizeof(*manager));
	if (manager == NULL) {
		wlr_log(WLR_ERROR, "Failed to allocate manager");
		return NULL;
	}

	manager->global =
		wl_global_create(display, &zwp_input_timestamps_manager_v1_interface,
		INPUT_TIMESTAMPS_MANAGER_VERSION, manager, manager_bind);
	if (manager->global == NULL) {
		wlr_log(WLR_ERROR, "Failed to create global");
		free(manager);
		return NULL;
	}

	wl_signal_init(&manager->events.destroy);

	manager->display_destroy.notify = handle_display_destroy;
	wl_display_add_destroy_listener(display, &manager->display_destroy);

	return manager;
}
