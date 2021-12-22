// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include <dlfcn.h>
#include <errno.h>
#include <gsl/gsl>
#include <wayland-egl.h>
#include <wayland-cursor.h>
#include <iostream>

#define LOAD_SYMBOL(handle, func) LoadSymbol(handle, #func, func)
#define LOAD_SYMBOL_SILENT(handle, func) LoadSymbol(handle, #func, func, true)

namespace Wayland {
namespace {

constexpr auto WL_CLOSURE_MAX_ARGS = 20;

struct wl_egl_window *(*wl_egl_window_create)(
	struct wl_surface *surface,
	int width,
	int height);
void (*wl_egl_window_destroy)(struct wl_egl_window *egl_window);
void (*wl_egl_window_resize)(
	struct wl_egl_window *egl_window,
	int width,
	int height,
	int dx,
	int dy);
void (*wl_egl_window_get_attached_size)(
	struct wl_egl_window *egl_window,
	int *width,
	int *height);

struct wl_buffer *(*wl_cursor_image_get_buffer)(
	struct wl_cursor_image *image);
struct wl_cursor_theme *(*wl_cursor_theme_load)(
	const char *name,
	int size,
	struct wl_shm *shm);
void (*wl_cursor_theme_destroy)(struct wl_cursor_theme *theme);
struct wl_cursor *(*wl_cursor_theme_get_cursor)(
	struct wl_cursor_theme *theme,
	const char *name);

int (*wl_cursor_frame_and_duration)(
	struct wl_cursor *cursor,
	uint32_t time,
	uint32_t *duration);

void (*wl_proxy_destroy)(struct wl_proxy *proxy);
uint32_t (*wl_proxy_get_version)(struct wl_proxy *proxy);
void (*wl_array_init)(struct wl_array *array);
void (*wl_array_release)(struct wl_array *array);
void *(*wl_array_add)(struct wl_array *array, size_t size);
int (*wl_proxy_add_listener)(
	struct wl_proxy *proxy,
	void (**implementation)(void),
	void *data);
void (*wl_proxy_set_user_data)(struct wl_proxy *proxy, void *user_data);
void *(*wl_proxy_get_user_data)(struct wl_proxy *proxy);
int (*wl_display_get_fd)(struct wl_display *display);
int (*wl_display_get_error)(struct wl_display *display);
int (*wl_display_dispatch)(struct wl_display *display);
struct wl_event_queue *(*wl_display_create_queue)(struct wl_display *display);
int (*wl_display_dispatch_queue_pending)(
	struct wl_display *display,
	struct wl_event_queue *queue);
int (*wl_display_prepare_read_queue)(
	struct wl_display *display,
	struct wl_event_queue *queue);
int (*wl_display_flush)(struct wl_display *display);
void (*wl_display_cancel_read)(struct wl_display *display);
int (*wl_display_read_events)(struct wl_display *display);
void (*wl_event_queue_destroy)(struct wl_event_queue *queue);
int (*wl_display_prepare_read)(struct wl_display *display);
int (*wl_display_dispatch_pending)(struct wl_display *display);
struct wl_display *(*wl_display_connect)(const char *name);
struct wl_display *(*wl_display_connect_to_fd)(int fd);
void (*wl_display_disconnect)(struct wl_display *display);
void *(*wl_proxy_create_wrapper)(void *proxy);
void (*wl_proxy_wrapper_destroy)(void *proxy_wrapper);
void (*wl_proxy_set_queue)(struct wl_proxy *proxy, struct wl_event_queue *queue);
int (*wl_display_roundtrip)(struct wl_display *display);
struct wl_proxy *(*wl_proxy_marshal_array_constructor)(
	struct wl_proxy *proxy,
	uint32_t opcode,
	union wl_argument *args,
	const struct wl_interface *interface);
struct wl_proxy *(*wl_proxy_marshal_array_constructor_versioned)(
	struct wl_proxy *proxy,
	uint32_t opcode,
	union wl_argument *args,
	const struct wl_interface *interface,
	uint32_t version);
struct wl_proxy *(*wl_proxy_marshal_array_flags)(
	struct wl_proxy *proxy,
	uint32_t opcode,
	const struct wl_interface *interface,
	uint32_t version,
	uint32_t flags,
	union wl_argument *args);
uint32_t (*wl_proxy_get_id)(struct wl_proxy *proxy);
const void *(*wl_proxy_get_listener)(struct wl_proxy *proxy);

struct argument_details {
	char type;
	int nullable;
};

const char *get_next_argument(
		const char *signature,
		struct argument_details *details) {
	details->nullable = 0;
	for(; *signature; ++signature) {
		switch(*signature) {
		case 'i':
		case 'u':
		case 'f':
		case 's':
		case 'o':
		case 'n':
		case 'a':
		case 'h':
			details->type = *signature;
			return signature + 1;
		case '?':
			details->nullable = 1;
		}
	}
	details->type = '\0';
	return signature;
}

void wl_argument_from_va_list(
		const char *signature,
		union wl_argument *args,
		int count,
		va_list ap) {
	int i;
	const char *sig_iter;
	struct argument_details arg;

	sig_iter = signature;
	for (i = 0; i < count; i++) {
		sig_iter = get_next_argument(sig_iter, &arg);

		switch(arg.type) {
		case 'i':
			args[i].i = va_arg(ap, int32_t);
			break;
		case 'u':
			args[i].u = va_arg(ap, uint32_t);
			break;
		case 'f':
			args[i].f = va_arg(ap, wl_fixed_t);
			break;
		case 's':
			args[i].s = va_arg(ap, const char *);
			break;
		case 'o':
			args[i].o = va_arg(ap, struct wl_object *);
			break;
		case 'n':
			args[i].o = va_arg(ap, struct wl_object *);
			break;
		case 'a':
			args[i].a = va_arg(ap, struct wl_array *);
			break;
		case 'h':
			args[i].h = va_arg(ap, int32_t);
			break;
		case '\0':
			return;
		}
	}
}

struct HandleDeleter {
	void operator()(void *handle) {
		dlclose(handle);
	}
};

using Handle = std::unique_ptr<void, HandleDeleter>;

bool LoadLibrary(Handle &handle, const char *name) {
	handle = Handle(dlopen(name, RTLD_LAZY | RTLD_NODELETE));
	if (handle) {
		return true;
	}
	std::cerr << dlerror() << std::endl;
	return false;
}

template <typename Function>
inline bool LoadSymbol(
		const Handle &handle,
		const char *name,
		Function &func,
		bool silent = false) {
	func = handle
		? reinterpret_cast<Function>(dlsym(handle.get(), name))
		: nullptr;
	if (const auto error = dlerror(); error && !silent) {
		std::cerr << error << std::endl;
	}
	return (func != nullptr);
}

bool Resolve() {
	static const auto loaded = [] {
		auto egl = Handle();
		auto cursor = Handle();
		auto client = Handle();
		const auto required = LoadLibrary(egl, "libwayland-egl.so.1")
			&& LOAD_SYMBOL(egl, wl_egl_window_create)
			&& LOAD_SYMBOL(egl, wl_egl_window_destroy)
			&& LOAD_SYMBOL(egl, wl_egl_window_resize)
			&& LOAD_SYMBOL(egl, wl_egl_window_get_attached_size)
			&& LoadLibrary(cursor, "libwayland-cursor.so.0")
			&& LOAD_SYMBOL(cursor, wl_cursor_image_get_buffer)
			&& LOAD_SYMBOL(cursor, wl_cursor_theme_load)
			&& LOAD_SYMBOL(cursor, wl_cursor_theme_destroy)
			&& LOAD_SYMBOL(cursor, wl_cursor_theme_get_cursor)
			&& LOAD_SYMBOL(cursor, wl_cursor_frame_and_duration)
			&& LoadLibrary(client, "libwayland-client.so.0")
			&& LOAD_SYMBOL(client, wl_proxy_destroy)
			&& LOAD_SYMBOL(client, wl_proxy_get_version)
			&& LOAD_SYMBOL(client, wl_array_init)
			&& LOAD_SYMBOL(client, wl_array_release)
			&& LOAD_SYMBOL(client, wl_array_add)
			&& LOAD_SYMBOL(client, wl_proxy_add_listener)
			&& LOAD_SYMBOL(client, wl_proxy_set_user_data)
			&& LOAD_SYMBOL(client, wl_proxy_get_user_data)
			&& LOAD_SYMBOL(client, wl_display_get_fd)
			&& LOAD_SYMBOL(client, wl_display_get_error)
			&& LOAD_SYMBOL(client, wl_display_dispatch)
			&& LOAD_SYMBOL(client, wl_display_create_queue)
			&& LOAD_SYMBOL(client, wl_display_dispatch_queue_pending)
			&& LOAD_SYMBOL(client, wl_display_prepare_read_queue)
			&& LOAD_SYMBOL(client, wl_display_flush)
			&& LOAD_SYMBOL(client, wl_display_cancel_read)
			&& LOAD_SYMBOL(client, wl_display_read_events)
			&& LOAD_SYMBOL(client, wl_event_queue_destroy)
			&& LOAD_SYMBOL(client, wl_display_prepare_read)
			&& LOAD_SYMBOL(client, wl_display_dispatch_pending)
			&& LOAD_SYMBOL(client, wl_display_connect)
			&& LOAD_SYMBOL(client, wl_display_connect_to_fd)
			&& LOAD_SYMBOL(client, wl_display_disconnect)
			&& LOAD_SYMBOL(client, wl_proxy_create_wrapper)
			&& LOAD_SYMBOL(client, wl_proxy_wrapper_destroy)
			&& LOAD_SYMBOL(client, wl_proxy_set_queue)
			&& LOAD_SYMBOL(client, wl_display_roundtrip)
			&& LOAD_SYMBOL(client, wl_proxy_marshal_array_constructor)
			&& LOAD_SYMBOL(client, wl_proxy_marshal_array_constructor_versioned)
			&& LOAD_SYMBOL(client, wl_proxy_get_id)
			&& LOAD_SYMBOL(client, wl_proxy_get_listener);
		LOAD_SYMBOL_SILENT(client, wl_proxy_marshal_array_flags);
		return required;
	}();
	return loaded;
}

} // namespace
} // namespace Wayland

namespace W = Wayland;

extern "C" {

struct wl_egl_window *wl_egl_window_create(
		struct wl_surface *surface,
		int width,
		int height) {
	Expects(W::wl_egl_window_create != nullptr);

	return W::wl_egl_window_create(surface, width, height);
}

void wl_egl_window_destroy(struct wl_egl_window *egl_window) {
	Expects(W::wl_egl_window_destroy != nullptr);

	W::wl_egl_window_destroy(egl_window);
}

void wl_egl_window_resize(
		struct wl_egl_window *egl_window,
		int width,
		int height,
		int dx,
		int dy) {
	Expects(W::wl_egl_window_resize != nullptr);

	W::wl_egl_window_resize(egl_window, width, height, dx, dy);
}

void wl_egl_window_get_attached_size(
		struct wl_egl_window *egl_window,
		int *width,
		int *height) {
	Expects(W::wl_egl_window_get_attached_size != nullptr);

	W::wl_egl_window_get_attached_size(egl_window, width, height);
}

struct wl_buffer *wl_cursor_image_get_buffer(struct wl_cursor_image *image) {
	Expects(W::wl_cursor_image_get_buffer != nullptr);

	return W::wl_cursor_image_get_buffer(image);
}

struct wl_cursor_theme *wl_cursor_theme_load(
		const char *name,
		int size,
		struct wl_shm *shm) {
	Expects(W::wl_cursor_theme_load != nullptr);

	return W::wl_cursor_theme_load(name, size, shm);
}

void wl_cursor_theme_destroy(struct wl_cursor_theme *theme) {
	Expects(W::wl_cursor_theme_destroy != nullptr);

	W::wl_cursor_theme_destroy(theme);
}

struct wl_cursor *wl_cursor_theme_get_cursor(
		struct wl_cursor_theme *theme,
		const char *name) {
	Expects(W::wl_cursor_theme_get_cursor != nullptr);

	return W::wl_cursor_theme_get_cursor(theme, name);
}

int wl_cursor_frame_and_duration(
		struct wl_cursor *cursor,
		uint32_t time,
		uint32_t *duration) {
	Expects(W::wl_cursor_frame_and_duration != nullptr);

	return W::wl_cursor_frame_and_duration(cursor, time, duration);
}

void wl_proxy_destroy(struct wl_proxy *proxy) {
	Expects(W::wl_proxy_destroy != nullptr);

	W::wl_proxy_destroy(proxy);
}

uint32_t wl_proxy_get_version(struct wl_proxy *proxy) {
	Expects(W::wl_proxy_get_version != nullptr);

	return W::wl_proxy_get_version(proxy);
}

void wl_array_init(struct wl_array *array) {
	Expects(W::wl_array_init != nullptr);

	W::wl_array_init(array);
}

void wl_array_release(struct wl_array *array) {
	Expects(W::wl_array_release != nullptr);

	W::wl_array_release(array);
}

void *wl_array_add(struct wl_array *array, size_t size) {
	Expects(W::wl_array_add != nullptr);

	return W::wl_array_add(array, size);
}

int wl_proxy_add_listener(
		struct wl_proxy *proxy,
		void (**implementation)(void),
		void *data) {
	Expects(W::wl_proxy_add_listener != nullptr);

	return W::wl_proxy_add_listener(proxy, implementation, data);
}

void wl_proxy_set_user_data(struct wl_proxy *proxy, void *user_data) {
	Expects(W::wl_proxy_set_user_data != nullptr);

	W::wl_proxy_set_user_data(proxy, user_data);
}

void *wl_proxy_get_user_data(struct wl_proxy *proxy) {
	Expects(W::wl_proxy_get_user_data != nullptr);

	return W::wl_proxy_get_user_data(proxy);
}

int wl_display_get_fd(struct wl_display *display) {
	Expects(W::wl_display_get_fd != nullptr);

	return W::wl_display_get_fd(display);
}

int wl_display_get_error(struct wl_display *display) {
	Expects(W::wl_display_get_error != nullptr);

	return W::wl_display_get_error(display);
}

int wl_display_dispatch(struct wl_display *display) {
	Expects(W::wl_display_dispatch != nullptr);

	return W::wl_display_dispatch(display);
}

struct wl_event_queue *wl_display_create_queue(struct wl_display *display) {
	Expects(W::wl_display_create_queue != nullptr);

	return W::wl_display_create_queue(display);
}

int wl_display_dispatch_queue_pending(
		struct wl_display *display,
		struct wl_event_queue *queue) {
	Expects(W::wl_display_dispatch_queue_pending != nullptr);

	return W::wl_display_dispatch_queue_pending(display, queue);
}

int wl_display_prepare_read_queue(
		struct wl_display *display,
		struct wl_event_queue *queue) {
	Expects(W::wl_display_prepare_read_queue != nullptr);

	return W::wl_display_prepare_read_queue(display, queue);
}

int wl_display_flush(struct wl_display *display) {
	Expects(W::wl_display_flush != nullptr);

	return W::wl_display_flush(display);
}

void wl_display_cancel_read(struct wl_display *display) {
	Expects(W::wl_display_cancel_read != nullptr);

	W::wl_display_cancel_read(display);
}

int wl_display_read_events(struct wl_display *display) {
	Expects(W::wl_display_read_events != nullptr);

	return W::wl_display_read_events(display);
}

void wl_event_queue_destroy(struct wl_event_queue *queue) {
	Expects(W::wl_event_queue_destroy != nullptr);

	W::wl_event_queue_destroy(queue);
}

int wl_display_prepare_read(struct wl_display *display) {
	Expects(W::wl_display_prepare_read != nullptr);

	return W::wl_display_prepare_read(display);
}

int wl_display_dispatch_pending(struct wl_display *display) {
	Expects(W::wl_display_dispatch_pending != nullptr);

	return W::wl_display_dispatch_pending(display);
}

struct wl_display *wl_display_connect(const char *name) {
	if (!W::Resolve()) {
		errno = ENOENT;
		return nullptr;
	}
	return W::wl_display_connect(name);
}

struct wl_display *wl_display_connect_to_fd(int fd) {
	if (!W::Resolve()) {
		errno = ENOENT;
		return nullptr;
	}
	return W::wl_display_connect_to_fd(fd);
}

void wl_display_disconnect(struct wl_display *display) {
	Expects(W::wl_display_disconnect != nullptr);

	W::wl_display_disconnect(display);
}

void *wl_proxy_create_wrapper(void *proxy) {
	Expects(W::wl_proxy_create_wrapper != nullptr);

	return W::wl_proxy_create_wrapper(proxy);
}

void wl_proxy_wrapper_destroy(void *proxy_wrapper) {
	Expects(W::wl_proxy_wrapper_destroy != nullptr);

	W::wl_proxy_wrapper_destroy(proxy_wrapper);
}

void wl_proxy_set_queue(
		struct wl_proxy *proxy,
		struct wl_event_queue *queue) {
	Expects(W::wl_proxy_set_queue != nullptr);

	W::wl_proxy_set_queue(proxy, queue);
}

int wl_display_roundtrip(struct wl_display *display) {
	Expects(W::wl_display_roundtrip != nullptr);

	return W::wl_display_roundtrip(display);
}

struct wl_proxy *wl_proxy_marshal_array_constructor(
		struct wl_proxy *proxy,
		uint32_t opcode,
		union wl_argument *args,
		const struct wl_interface *interface) {
	Expects(W::wl_proxy_marshal_array_constructor != nullptr);

	return W::wl_proxy_marshal_array_constructor(
		proxy,
		opcode,
		args,
		interface);
}

struct wl_proxy *wl_proxy_marshal_array_constructor_versioned(
		struct wl_proxy *proxy,
		uint32_t opcode,
		union wl_argument *args,
		const struct wl_interface *interface,
		uint32_t version) {
	Expects(W::wl_proxy_marshal_array_constructor_versioned != nullptr);

	return W::wl_proxy_marshal_array_constructor_versioned(
		proxy,
		opcode,
		args,
		interface,
		version);
}

struct wl_proxy *wl_proxy_marshal_array_flags(
		struct wl_proxy *proxy,
		uint32_t opcode,
		const struct wl_interface *interface,
		uint32_t version,
		uint32_t flags,
		union wl_argument *args) {
	Expects(W::wl_proxy_marshal_array_flags != nullptr);

	return W::wl_proxy_marshal_array_flags(
		proxy,
		opcode,
		interface,
		version,
		flags,
		args);
}

uint32_t wl_proxy_get_id(struct wl_proxy *proxy) {
	Expects(W::wl_proxy_get_id != nullptr);

	return W::wl_proxy_get_id(proxy);
}

const void *wl_proxy_get_listener(struct wl_proxy *proxy) {
	Expects(W::wl_proxy_get_listener != nullptr);

	return W::wl_proxy_get_listener(proxy);
}

void wl_proxy_marshal(struct wl_proxy *proxy, uint32_t opcode, ...) {
	union wl_argument args[W::WL_CLOSURE_MAX_ARGS];
	va_list ap;

	va_start(ap, opcode);

	// wl_proxy { wl_object { wl_interface *, ... }, ... }
	struct wl_object *object = (struct wl_object*)proxy;
	struct wl_interface *interface = *((struct wl_interface**)object);
	W::wl_argument_from_va_list(
		interface->methods[opcode].signature,
		args,
		W::WL_CLOSURE_MAX_ARGS,
		ap);

	va_end(ap);

	wl_proxy_marshal_array_constructor(proxy, opcode, args, nullptr);
}

struct wl_proxy *wl_proxy_marshal_constructor(
		struct wl_proxy *proxy,
		uint32_t opcode,
		const struct wl_interface *interface,
		...) {
	union wl_argument args[W::WL_CLOSURE_MAX_ARGS];
	va_list ap;

	va_start(ap, interface);

	// wl_proxy { wl_object { wl_interface *, ... }, ... }
	struct wl_object *object = (struct wl_object*)proxy;
	struct wl_interface *i = *((struct wl_interface**)object);
	W::wl_argument_from_va_list(
		i->methods[opcode].signature,
		args,
		W::WL_CLOSURE_MAX_ARGS,
		ap);

	va_end(ap);

	return wl_proxy_marshal_array_constructor(proxy, opcode, args, interface);
}

struct wl_proxy *wl_proxy_marshal_constructor_versioned(
		struct wl_proxy *proxy,
		uint32_t opcode,
		const struct wl_interface *interface,
		uint32_t version,
		...) {
	union wl_argument args[W::WL_CLOSURE_MAX_ARGS];
	va_list ap;

	va_start(ap, version);

	// wl_proxy { wl_object { wl_interface *, ... }, ... }
	struct wl_object *object = (struct wl_object*)proxy;
	struct wl_interface *i = *((struct wl_interface**)object);
	W::wl_argument_from_va_list(
		i->methods[opcode].signature,
		args,
		W::WL_CLOSURE_MAX_ARGS,
		ap);

	va_end(ap);

	return wl_proxy_marshal_array_constructor_versioned(proxy, opcode,
							    args, interface,
							    version);
}

struct wl_proxy *wl_proxy_marshal_flags(
		struct wl_proxy *proxy,
		uint32_t opcode,
		const struct wl_interface *interface,
		uint32_t version,
		uint32_t flags,
		...) {
	union wl_argument args[W::WL_CLOSURE_MAX_ARGS];
	va_list ap;

	va_start(ap, flags);

	// wl_proxy { wl_object { wl_interface *, ... }, ... }
	struct wl_object *object = (struct wl_object*)proxy;
	struct wl_interface *i = *((struct wl_interface**)object);
	W::wl_argument_from_va_list(
		i->methods[opcode].signature,
		args,
		W::WL_CLOSURE_MAX_ARGS,
		ap);

	va_end(ap);

	if (W::wl_proxy_marshal_array_flags == nullptr) {
		const auto result = wl_proxy_marshal_array_constructor_versioned(
			proxy,
			opcode,
			args,
			interface,
			version);
		
		if (flags & WL_MARSHAL_FLAG_DESTROY) {
			wl_proxy_destroy(proxy);
		}

		return result;
	}

	return wl_proxy_marshal_array_flags(
		proxy,
		opcode,
		interface,
		version,
		flags,
		args);
}

} // extern "C"
