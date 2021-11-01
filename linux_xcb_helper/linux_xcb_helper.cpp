// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include <dlfcn.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xcbext.h>

unsigned int xcb_send_request_with_fds(
		xcb_connection_t *c,
		int flags,
		struct iovec *vector,
		const xcb_protocol_request_t *req,
		unsigned int num_fds,
		int *fds) {
	const auto send_request_with_fds = reinterpret_cast<unsigned int(*)(
		xcb_connection_t*,
		int,
		struct iovec*,
		const xcb_protocol_request_t*,
		unsigned int,
		int*)>(dlsym(RTLD_NEXT, "xcb_send_request_with_fds"));

	if (!dlerror()) {
		return send_request_with_fds(c, flags, vector, req, num_fds, fds);
	}

	const auto send_fd = reinterpret_cast<void(*)(xcb_connection_t*, int)>(
		dlsym(RTLD_NEXT, "xcb_send_fd"));

	if (dlerror()) {
		abort();
	}

	const auto send_request = reinterpret_cast<unsigned int(*)(
		xcb_connection_t*,
		int,
		struct iovec*,
		const xcb_protocol_request_t*)>(dlsym(RTLD_NEXT, "xcb_send_request"));

	if (dlerror()) {
		abort();
	}

	for (int i = 0; i != num_fds; ++i) {
		send_fd(c, fds[i]);
	}

	return send_request(c, flags, vector, req);
}
