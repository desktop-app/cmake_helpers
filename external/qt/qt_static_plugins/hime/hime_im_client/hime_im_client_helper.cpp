// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include "base/platform/linux/base_linux_library.h"

#include <X11/Xlib.h>
#include <hime-im-client.h>

namespace HimeHelper {
namespace {

void (*hime_im_client_close)(HIME_client_handle *handle);
void (*hime_im_client_focus_in)(HIME_client_handle *handle);
void (*hime_im_client_focus_out)(HIME_client_handle *handle);
void (*hime_im_client_focus_out2)(HIME_client_handle *handle, char **rstr);
int (*hime_im_client_forward_key_press)(
	HIME_client_handle *handle,
	const KeySym key,
	const uint32_t state,
	char **rstr);
int (*hime_im_client_forward_key_release)(
	HIME_client_handle *handle,
	const KeySym key,
	const uint32_t state,
	char **rstr);
int (*hime_im_client_get_preedit)(
	HIME_client_handle *handle,
	char **str,
	HIME_PREEDIT_ATTR att[],
	int *cursor,
	int *sub_comp_len);
HIME_client_handle *(*hime_im_client_open)(Display *display);
void (*hime_im_client_reset)(HIME_client_handle *handle);
void (*hime_im_client_set_cursor_location)(
	HIME_client_handle *handle,
	const int x,
	const int y);
void (*hime_im_client_set_flags)(
	HIME_client_handle *handle,
	const int flags,
	int *ret_flags);
void (*hime_im_client_set_client_window)(
	HIME_client_handle *handle,
	const Window win);
void (*hime_im_client_set_window)(HIME_client_handle *handle, Window win);

bool Resolve() {
	static const auto loaded = [&] {
		const auto lib = base::Platform::LoadLibrary(
			"libhime-im-client.so.1",
			RTLD_NODELETE);
		return lib
			&& LOAD_LIBRARY_SYMBOL(lib, hime_im_client_close)
			&& LOAD_LIBRARY_SYMBOL(lib, hime_im_client_focus_in)
			&& LOAD_LIBRARY_SYMBOL(lib, hime_im_client_focus_out)
			&& LOAD_LIBRARY_SYMBOL(lib, hime_im_client_focus_out2)
			&& LOAD_LIBRARY_SYMBOL(lib, hime_im_client_forward_key_press)
			&& LOAD_LIBRARY_SYMBOL(lib, hime_im_client_forward_key_release)
			&& LOAD_LIBRARY_SYMBOL(lib, hime_im_client_get_preedit)
			&& LOAD_LIBRARY_SYMBOL(lib, hime_im_client_open)
			&& LOAD_LIBRARY_SYMBOL(lib, hime_im_client_reset)
			&& LOAD_LIBRARY_SYMBOL(lib, hime_im_client_set_cursor_location)
			&& LOAD_LIBRARY_SYMBOL(lib, hime_im_client_set_flags)
			&& (LOAD_LIBRARY_SYMBOL(lib, hime_im_client_set_client_window)
				|| LOAD_LIBRARY_SYMBOL(lib, hime_im_client_set_window));
	}();
	return loaded;
}

} // namespace
} // namespace HimeHelper

void hime_im_client_close(HIME_client_handle *handle) {
	HimeHelper::Resolve();
	HimeHelper::hime_im_client_close(handle);
}

void hime_im_client_focus_in(HIME_client_handle *handle) {
	HimeHelper::Resolve();
	HimeHelper::hime_im_client_focus_in(handle);
}

void hime_im_client_focus_out(HIME_client_handle *handle) {
	HimeHelper::Resolve();
	HimeHelper::hime_im_client_focus_out(handle);
}

void hime_im_client_focus_out2(HIME_client_handle *handle, char **rstr) {
	HimeHelper::Resolve();
	HimeHelper::hime_im_client_focus_out2(handle, rstr);
}

int hime_im_client_forward_key_press(
	HIME_client_handle *handle,
	const KeySym key,
	const uint32_t state,
	char **rstr) {
	HimeHelper::Resolve();
	return HimeHelper::hime_im_client_forward_key_press(
		handle,
		key,
		state,
		rstr);
}

int hime_im_client_forward_key_release(
	HIME_client_handle *handle,
	const KeySym key,
	const uint32_t state,
	char **rstr) {
	HimeHelper::Resolve();
	return HimeHelper::hime_im_client_forward_key_release(
		handle,
		key,
		state,
		rstr);
}

int hime_im_client_get_preedit(
	HIME_client_handle *handle,
	char **str,
	HIME_PREEDIT_ATTR att[],
	int *cursor,
	int *sub_comp_len) {
	HimeHelper::Resolve();
	return HimeHelper::hime_im_client_get_preedit(
		handle,
		str,
		att,
		cursor,
		sub_comp_len);
}

HIME_client_handle *hime_im_client_open(Display *display) {
	HimeHelper::Resolve();
	return HimeHelper::hime_im_client_open(display);
}

void hime_im_client_reset(HIME_client_handle *handle) {
	HimeHelper::Resolve();
	HimeHelper::hime_im_client_reset(handle);
}

void hime_im_client_set_cursor_location(
	HIME_client_handle *handle,
	const int x,
	const int y) {
	HimeHelper::Resolve();
	HimeHelper::hime_im_client_set_cursor_location(
		handle,
		x,
		y);
}

void hime_im_client_set_flags(
	HIME_client_handle *handle,
	const int flags,
	int *ret_flags) {
	HimeHelper::Resolve();
	HimeHelper::hime_im_client_set_flags(
		handle,
		flags,
		ret_flags);
}

void hime_im_client_set_client_window(
	HIME_client_handle *handle,
	const Window win) {
	HimeHelper::Resolve();
	HimeHelper::hime_im_client_set_client_window(handle, win);
}

void hime_im_client_set_window(HIME_client_handle *handle, Window win) {
	HimeHelper::Resolve();
	HimeHelper::hime_im_client_set_window(handle, win);
}
