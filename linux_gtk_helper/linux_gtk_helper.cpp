// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include <dlfcn.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <memory>
#include <iostream>

#define LOAD_SYMBOL(handle, func) LoadSymbol(handle, #func, func)

namespace GtkHelper {
namespace {

GdkPixbuf *(*gdk_pixbuf_new_from_file_at_size)(
	const char *filename,
	int width,
	int height,
	GError **error);
void (*gdk_window_focus)(GdkWindow *window, guint32 timestamp);
GdkDisplay *(*gdk_window_get_display)(GdkWindow *window);
void (*gdk_window_set_modal_hint)(GdkWindow *window, gboolean modal);
Display *(*gdk_x11_display_get_xdisplay)(GdkDisplay *display);
GType (*gdk_x11_window_get_type)(void);
Window (*gdk_x11_window_get_xid)(GdkWindow *window);
GType (*gtk_accel_label_get_type)(void);
void (*gtk_accel_label_set_accel)(
	GtkAccelLabel *accel_label,
	guint accelerator_key,
	GdkModifierType accelerator_mods);
GtkWidget *(*gtk_bin_get_child)(GtkBin *bin);
GType (*gtk_bin_get_type)(void);
GType (*gtk_button_get_type)(void);
void (*gtk_button_set_label)(GtkButton *button, const gchar *label);
gboolean (*gtk_check_menu_item_get_active)(GtkCheckMenuItem *check_menu_item);
GType (*gtk_check_menu_item_get_type)(void);
GtkWidget* (*gtk_check_menu_item_new)(void);
void (*gtk_check_menu_item_set_active)(
	GtkCheckMenuItem *check_menu_item,
	gboolean is_active);
const gchar *(*gtk_check_version)(
	guint required_major,
	guint required_minor,
	guint required_micro);
GtkClipboard *(*gtk_clipboard_get)(GdkAtom selection);
void (*gtk_clipboard_store)(GtkClipboard *clipboard);
GtkWidget *(*gtk_color_chooser_dialog_new)(
	const gchar *title,
	GtkWindow *parent);
GType (*gtk_color_chooser_get_type)(void);
void (*gtk_color_chooser_get_rgba)(
	GtkColorChooser *chooser,
	GdkRGBA *color);
void (*gtk_color_chooser_set_rgba)(
	GtkColorChooser *chooser,
	const GdkRGBA *color);
void (*gtk_color_chooser_set_use_alpha)(
	GtkColorChooser *chooser,
	gboolean use_alpha);
GType (*gtk_container_get_type)(void);
void (*gtk_container_remove)(GtkContainer *container, GtkWidget *widget);
GtkWidget *(*gtk_dialog_add_button)(
	GtkDialog *dialog,
	const gchar *button_text,
	gint response_id);
GType (*gtk_dialog_get_type)(void);
GtkWidget* (*gtk_dialog_get_widget_for_response)(
	GtkDialog *dialog,
	gint response_id);
gint (*gtk_dialog_run)(GtkDialog *dialog);
void (*gtk_file_chooser_add_filter)(
	GtkFileChooser *chooser,
	GtkFileFilter *filter);
GType (*gtk_file_chooser_dialog_get_type)(void);
gchar *(*gtk_file_chooser_get_current_folder)(GtkFileChooser *chooser);
gchar *(*gtk_file_chooser_get_filename)(GtkFileChooser *chooser);
GSList *(*gtk_file_chooser_get_filenames)(GtkFileChooser *chooser);
GtkFileFilter *(*gtk_file_chooser_get_filter)(GtkFileChooser *chooser);
char *(*gtk_file_chooser_get_preview_filename)(GtkFileChooser *chooser);
GType (*gtk_file_chooser_get_type)(void);
void (*gtk_file_chooser_remove_filter)(
	GtkFileChooser *chooser,
	GtkFileFilter *filter);
gboolean (*gtk_file_chooser_select_filename)(
	GtkFileChooser *chooser,
	const char *filename);
void (*gtk_file_chooser_set_action)(
	GtkFileChooser *chooser,
	GtkFileChooserAction action);
void (*gtk_file_chooser_set_create_folders)(
	GtkFileChooser *chooser,
	gboolean create_folders);
gboolean (*gtk_file_chooser_set_current_folder)(
	GtkFileChooser *chooser,
	const gchar *filename);
void (*gtk_file_chooser_set_current_name)(
	GtkFileChooser *chooser,
	const gchar *name);
void (*gtk_file_chooser_set_do_overwrite_confirmation)(
	GtkFileChooser *chooser,
	gboolean do_overwrite_confirmation);
void (*gtk_file_chooser_set_filter)(
	GtkFileChooser *chooser,
	GtkFileFilter *filter);
void (*gtk_file_chooser_set_local_only)(
	GtkFileChooser *chooser,
	gboolean local_only);
void (*gtk_file_chooser_set_preview_widget)(
	GtkFileChooser *chooser,
	GtkWidget *preview_widget);
void (*gtk_file_chooser_set_preview_widget_active)(
	GtkFileChooser *chooser,
	gboolean active);
void (*gtk_file_chooser_set_select_multiple)(
	GtkFileChooser *chooser,
	gboolean select_multiple);
void (*gtk_file_filter_add_pattern)(
	GtkFileFilter *filter,
	const gchar *pattern);
GtkFileFilter *(*gtk_file_filter_new)(void);
void (*gtk_file_filter_set_name)(
	GtkFileFilter *filter,
	const gchar *name);
GtkWidget* (*gtk_font_chooser_dialog_new)(
	const gchar *title,
	GtkWindow *parent);
gchar* (*gtk_font_chooser_get_font)(GtkFontChooser *fontchooser);
GType (*gtk_font_chooser_get_type)(void);
void (*gtk_font_chooser_set_font)(
	GtkFontChooser *fontchooser,
	const gchar *fontname);
guint32 (*gtk_get_current_event_time)(void);
GType (*gtk_image_get_type)(void);
GtkWidget* (*gtk_image_new)(void);
void (*gtk_image_set_from_pixbuf)(GtkImage *image, GdkPixbuf *pixbuf);
void (*gtk_init)(int *argc, char ***argv);
GType (*gtk_menu_get_type)(void);
GType (*gtk_menu_item_get_type)(void);
GtkWidget* (*gtk_menu_item_new)(void);
void (*gtk_menu_item_set_label)(GtkMenuItem *menu_item, const gchar *label);
void (*gtk_menu_item_set_submenu)(GtkMenuItem *menu_item, GtkWidget *submenu);
void (*gtk_menu_item_set_use_underline)(
	GtkMenuItem *menu_item,
	gboolean setting);
GtkWidget* (*gtk_menu_new)(void);
void (*gtk_menu_popdown)(GtkMenu *menu);
void (*gtk_menu_popup)(
	GtkMenu *menu,
	GtkWidget *parent_menu_shell,
	GtkWidget *parent_menu_item,
	GtkMenuPositionFunc func,
	gpointer data,
	guint button,
	guint32 activate_time);
GType (*gtk_menu_shell_get_type)(void);
void (*gtk_menu_shell_insert)(
	GtkMenuShell *menu_shell,
	GtkWidget *child,
	gint position);
void (*gtk_menu_shell_select_item)(
	GtkMenuShell *menu_shell,
	GtkWidget *menu_item);
GtkWidget* (*gtk_separator_menu_item_new)(void);
GtkSettings* (*gtk_settings_get_default)(void);
void (*gtk_widget_destroy)(GtkWidget *widget);
gint (*gtk_widget_get_scale_factor)(GtkWidget *widget);
GType (*gtk_widget_get_type)(void);
GdkWindow *(*gtk_widget_get_window)(GtkWidget *widget);
void (*gtk_widget_hide)(GtkWidget *widget);
gboolean (*gtk_widget_hide_on_delete)(GtkWidget *widget);
void (*gtk_widget_realize)(GtkWidget *widget);
void (*gtk_widget_set_sensitive)(GtkWidget *widget, gboolean sensitive);
void (*gtk_widget_set_visible)(GtkWidget *widget, gboolean visible);
void (*gtk_widget_show)(GtkWidget *widget);
GType (*gtk_window_get_type)(void);
void (*gtk_window_set_title)(GtkWindow *window, const gchar *title);
void (*gtk_window_set_transient_for)(GtkWindow *window, GtkWindow *parent);
void (*pango_font_description_free)(PangoFontDescription *desc);
PangoFontDescription *(*pango_font_description_from_string)(const char *str);
const char *(*pango_font_description_get_family)(
	const PangoFontDescription *desc);
gint (*pango_font_description_get_size)(const PangoFontDescription *desc);
PangoStyle (*pango_font_description_get_style)(
	const PangoFontDescription *desc);
PangoWeight (*pango_font_description_get_weight)(
	const PangoFontDescription *desc);
PangoFontDescription *(*pango_font_description_new)(void);
void (*pango_font_description_set_family)(
	PangoFontDescription *desc,
	const char *family);
void (*pango_font_description_set_size)(
	PangoFontDescription *desc,
	gint size);
void (*pango_font_description_set_style)(
	PangoFontDescription *desc,
	PangoStyle style);
void (*pango_font_description_set_weight)(
	PangoFontDescription *desc,
	PangoWeight weight);
char *(*pango_font_description_to_string)(const PangoFontDescription *desc);
GType (*pango_font_face_get_type)(void);
GType (*pango_font_family_get_type)(void);

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
inline bool LoadSymbol(const Handle &handle, const char *name, Function &func) {
	func = handle
		? reinterpret_cast<Function>(dlsym(handle.get(), name))
		: nullptr;
	if (const auto error = dlerror()) {
		std::cerr << error << std::endl;
	}
	return (func != nullptr);
}

bool Resolve() {
	static const auto loaded = [&] {
		auto lib = Handle();
		return LoadLibrary(lib, "libgtk-3.so.0")
			&& LOAD_SYMBOL(lib, gdk_pixbuf_new_from_file_at_size)
			&& LOAD_SYMBOL(lib, gdk_window_focus)
			&& LOAD_SYMBOL(lib, gdk_window_get_display)
			&& LOAD_SYMBOL(lib, gdk_window_set_modal_hint)
			&& LOAD_SYMBOL(lib, gdk_x11_display_get_xdisplay)
			&& LOAD_SYMBOL(lib, gdk_x11_window_get_type)
			&& LOAD_SYMBOL(lib, gdk_x11_window_get_xid)
			&& LOAD_SYMBOL(lib, gtk_accel_label_get_type)
			&& LOAD_SYMBOL(lib, gtk_accel_label_set_accel)
			&& LOAD_SYMBOL(lib, gtk_bin_get_child)
			&& LOAD_SYMBOL(lib, gtk_bin_get_type)
			&& LOAD_SYMBOL(lib, gtk_button_get_type)
			&& LOAD_SYMBOL(lib, gtk_button_set_label)
			&& LOAD_SYMBOL(lib, gtk_check_menu_item_get_active)
			&& LOAD_SYMBOL(lib, gtk_check_menu_item_get_type)
			&& LOAD_SYMBOL(lib, gtk_check_menu_item_new)
			&& LOAD_SYMBOL(lib, gtk_check_menu_item_set_active)
			&& LOAD_SYMBOL(lib, gtk_check_version)
			&& LOAD_SYMBOL(lib, gtk_clipboard_get)
			&& LOAD_SYMBOL(lib, gtk_clipboard_store)
			&& LOAD_SYMBOL(lib, gtk_color_chooser_dialog_new)
			&& LOAD_SYMBOL(lib, gtk_color_chooser_get_rgba)
			&& LOAD_SYMBOL(lib, gtk_color_chooser_get_type)
			&& LOAD_SYMBOL(lib, gtk_color_chooser_set_rgba)
			&& LOAD_SYMBOL(lib, gtk_color_chooser_set_use_alpha)
			&& LOAD_SYMBOL(lib, gtk_container_get_type)
			&& LOAD_SYMBOL(lib, gtk_container_remove)
			&& LOAD_SYMBOL(lib, gtk_dialog_add_button)
			&& LOAD_SYMBOL(lib, gtk_dialog_get_type)
			&& LOAD_SYMBOL(lib, gtk_dialog_get_widget_for_response)
			&& LOAD_SYMBOL(lib, gtk_dialog_run)
			&& LOAD_SYMBOL(lib, gtk_file_chooser_add_filter)
			&& LOAD_SYMBOL(lib, gtk_file_chooser_dialog_get_type)
			&& LOAD_SYMBOL(lib, gtk_file_chooser_get_current_folder)
			&& LOAD_SYMBOL(lib, gtk_file_chooser_get_filename)
			&& LOAD_SYMBOL(lib, gtk_file_chooser_get_filenames)
			&& LOAD_SYMBOL(lib, gtk_file_chooser_get_filter)
			&& LOAD_SYMBOL(lib, gtk_file_chooser_get_preview_filename)
			&& LOAD_SYMBOL(lib, gtk_file_chooser_get_type)
			&& LOAD_SYMBOL(lib, gtk_file_chooser_remove_filter)
			&& LOAD_SYMBOL(lib, gtk_file_chooser_select_filename)
			&& LOAD_SYMBOL(lib, gtk_file_chooser_set_action)
			&& LOAD_SYMBOL(lib, gtk_file_chooser_set_create_folders)
			&& LOAD_SYMBOL(lib, gtk_file_chooser_set_current_folder)
			&& LOAD_SYMBOL(lib, gtk_file_chooser_set_current_name)
			&& LOAD_SYMBOL(lib, gtk_file_chooser_set_do_overwrite_confirmation)
			&& LOAD_SYMBOL(lib, gtk_file_chooser_set_filter)
			&& LOAD_SYMBOL(lib, gtk_file_chooser_set_local_only)
			&& LOAD_SYMBOL(lib, gtk_file_chooser_set_preview_widget)
			&& LOAD_SYMBOL(lib, gtk_file_chooser_set_preview_widget_active)
			&& LOAD_SYMBOL(lib, gtk_file_chooser_set_select_multiple)
			&& LOAD_SYMBOL(lib, gtk_file_filter_add_pattern)
			&& LOAD_SYMBOL(lib, gtk_file_filter_new)
			&& LOAD_SYMBOL(lib, gtk_file_filter_set_name)
			&& LOAD_SYMBOL(lib, gtk_font_chooser_dialog_new)
			&& LOAD_SYMBOL(lib, gtk_font_chooser_get_font)
			&& LOAD_SYMBOL(lib, gtk_font_chooser_get_type)
			&& LOAD_SYMBOL(lib, gtk_font_chooser_set_font)
			&& LOAD_SYMBOL(lib, gtk_get_current_event_time)
			&& LOAD_SYMBOL(lib, gtk_image_get_type)
			&& LOAD_SYMBOL(lib, gtk_image_new)
			&& LOAD_SYMBOL(lib, gtk_image_set_from_pixbuf)
			&& LOAD_SYMBOL(lib, gtk_init)
			&& LOAD_SYMBOL(lib, gtk_menu_get_type)
			&& LOAD_SYMBOL(lib, gtk_menu_item_get_type)
			&& LOAD_SYMBOL(lib, gtk_menu_item_new)
			&& LOAD_SYMBOL(lib, gtk_menu_item_set_label)
			&& LOAD_SYMBOL(lib, gtk_menu_item_set_submenu)
			&& LOAD_SYMBOL(lib, gtk_menu_item_set_use_underline)
			&& LOAD_SYMBOL(lib, gtk_menu_new)
			&& LOAD_SYMBOL(lib, gtk_menu_popdown)
			&& LOAD_SYMBOL(lib, gtk_menu_popup)
			&& LOAD_SYMBOL(lib, gtk_menu_shell_get_type)
			&& LOAD_SYMBOL(lib, gtk_menu_shell_insert)
			&& LOAD_SYMBOL(lib, gtk_menu_shell_select_item)
			&& LOAD_SYMBOL(lib, gtk_separator_menu_item_new)
			&& LOAD_SYMBOL(lib, gtk_settings_get_default)
			&& LOAD_SYMBOL(lib, gtk_widget_destroy)
			&& LOAD_SYMBOL(lib, gtk_widget_get_scale_factor)
			&& LOAD_SYMBOL(lib, gtk_widget_get_type)
			&& LOAD_SYMBOL(lib, gtk_widget_get_window)
			&& LOAD_SYMBOL(lib, gtk_widget_hide)
			&& LOAD_SYMBOL(lib, gtk_widget_hide_on_delete)
			&& LOAD_SYMBOL(lib, gtk_widget_realize)
			&& LOAD_SYMBOL(lib, gtk_widget_set_sensitive)
			&& LOAD_SYMBOL(lib, gtk_widget_set_visible)
			&& LOAD_SYMBOL(lib, gtk_widget_show)
			&& LOAD_SYMBOL(lib, gtk_window_get_type)
			&& LOAD_SYMBOL(lib, gtk_window_set_title)
			&& LOAD_SYMBOL(lib, gtk_window_set_transient_for)
			&& LOAD_SYMBOL(lib, pango_font_description_free)
			&& LOAD_SYMBOL(lib, pango_font_description_from_string)
			&& LOAD_SYMBOL(lib, pango_font_description_get_family)
			&& LOAD_SYMBOL(lib, pango_font_description_get_size)
			&& LOAD_SYMBOL(lib, pango_font_description_get_style)
			&& LOAD_SYMBOL(lib, pango_font_description_get_weight)
			&& LOAD_SYMBOL(lib, pango_font_description_new)
			&& LOAD_SYMBOL(lib, pango_font_description_set_family)
			&& LOAD_SYMBOL(lib, pango_font_description_set_size)
			&& LOAD_SYMBOL(lib, pango_font_description_set_style)
			&& LOAD_SYMBOL(lib, pango_font_description_set_weight)
			&& LOAD_SYMBOL(lib, pango_font_description_to_string)
			&& LOAD_SYMBOL(lib, pango_font_face_get_type)
			&& LOAD_SYMBOL(lib, pango_font_family_get_type);
	}();
	return loaded;
}

} // namespace
} // namespace GtkHelper

GdkPixbuf *gdk_pixbuf_new_from_file_at_size(
	const char *filename,
	int width,
	int height,
	GError **error) {
	GtkHelper::Resolve();
	return GtkHelper::gdk_pixbuf_new_from_file_at_size(
		filename,
		width,
		height,
		error);
}

void gdk_window_focus(GdkWindow *window, guint32 timestamp) {
	GtkHelper::Resolve();
	return GtkHelper::gdk_window_focus(window, timestamp);
}

GdkDisplay *gdk_window_get_display(GdkWindow *window) {
	GtkHelper::Resolve();
	return GtkHelper::gdk_window_get_display(window);
}

void gdk_window_set_modal_hint(GdkWindow *window, gboolean modal) {
	GtkHelper::Resolve();
	GtkHelper::gdk_window_set_modal_hint(window, modal);
}

Display *gdk_x11_display_get_xdisplay(GdkDisplay *display) {
	GtkHelper::Resolve();
	return GtkHelper::gdk_x11_display_get_xdisplay(display);
}

GType gdk_x11_window_get_type(void) {
	GtkHelper::Resolve();
	return GtkHelper::gdk_x11_window_get_type();
}

Window gdk_x11_window_get_xid(GdkWindow *window) {
	GtkHelper::Resolve();
	return GtkHelper::gdk_x11_window_get_xid(window);
}

GType gtk_accel_label_get_type(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_accel_label_get_type();
}

void gtk_accel_label_set_accel(
	GtkAccelLabel *accel_label,
	guint accelerator_key,
	GdkModifierType accelerator_mods) {
	GtkHelper::Resolve();
	GtkHelper::gtk_accel_label_set_accel(
		accel_label,
		accelerator_key,
		accelerator_mods);
}

GtkWidget *gtk_bin_get_child(GtkBin *bin) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_bin_get_child(bin);
}

GType gtk_bin_get_type(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_bin_get_type();
}

GType gtk_button_get_type(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_button_get_type();
}

void gtk_button_set_label(GtkButton *button, const gchar *label) {
	GtkHelper::Resolve();
	GtkHelper::gtk_button_set_label(button, label);
}

gboolean gtk_check_menu_item_get_active(GtkCheckMenuItem *check_menu_item) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_check_menu_item_get_active(check_menu_item);
}

GType gtk_check_menu_item_get_type(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_check_menu_item_get_type();
}

GtkWidget* gtk_check_menu_item_new(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_check_menu_item_new();
}

void gtk_check_menu_item_set_active(
	GtkCheckMenuItem *check_menu_item,
	gboolean is_active) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_check_menu_item_set_active(
		check_menu_item,
		is_active);
}

const gchar *gtk_check_version(
	guint required_major,
	guint required_minor,
	guint required_micro) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_check_version(
		required_major,
		required_minor,
		required_micro);
}

GtkClipboard *gtk_clipboard_get(GdkAtom selection) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_clipboard_get(selection);
}

void gtk_clipboard_store(GtkClipboard *clipboard) {
	GtkHelper::Resolve();
	GtkHelper::gtk_clipboard_store(clipboard);
}

GtkWidget *gtk_color_chooser_dialog_new(
	const gchar *title,
	GtkWindow *parent) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_color_chooser_dialog_new(title, parent);
}

GType gtk_color_chooser_get_type(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_color_chooser_get_type();
}

void gtk_color_chooser_get_rgba(
	GtkColorChooser *chooser,
	GdkRGBA *color) {
	GtkHelper::Resolve();
	GtkHelper::gtk_color_chooser_get_rgba(chooser, color);
}

void gtk_color_chooser_set_rgba(
	GtkColorChooser *chooser,
	const GdkRGBA *color) {
	GtkHelper::Resolve();
	GtkHelper::gtk_color_chooser_set_rgba(chooser, color);
}

void gtk_color_chooser_set_use_alpha(
	GtkColorChooser *chooser,
	gboolean use_alpha) {
	GtkHelper::Resolve();
	GtkHelper::gtk_color_chooser_set_use_alpha(chooser, use_alpha);
}

GType gtk_container_get_type(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_container_get_type();
}

void gtk_container_remove(GtkContainer *container, GtkWidget *widget) {
	GtkHelper::Resolve();
	GtkHelper::gtk_container_remove(container, widget);
}

GType gtk_dialog_get_type(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_dialog_get_type();
}

GtkWidget* gtk_dialog_get_widget_for_response(
	GtkDialog *dialog,
	gint response_id) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_dialog_get_widget_for_response(
		dialog,
		response_id);
}

gint gtk_dialog_run(GtkDialog *dialog) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_dialog_run(dialog);
}

void gtk_file_chooser_add_filter(
	GtkFileChooser *chooser,
	GtkFileFilter *filter) {
	GtkHelper::Resolve();
	GtkHelper::gtk_file_chooser_add_filter(chooser, filter);
}

GtkWidget *gtk_file_chooser_dialog_new(
	const gchar *title,
	GtkWindow *parent,
	GtkFileChooserAction action,
	const gchar *first_button_text,
	...) {
	GtkHelper::Resolve();

	va_list varargs;
	va_start(varargs, first_button_text);

	const auto result = static_cast<GtkWidget*>(g_object_new(
		GtkHelper::gtk_file_chooser_dialog_get_type(),
		"title",
		title,
		"action",
		action,
		nullptr));

	if (parent) {
		GtkHelper::gtk_window_set_transient_for(GTK_WINDOW(result), parent);
	}

	auto button_text = first_button_text;
	gint response_id;
	while (button_text) {
		response_id = va_arg(varargs, gint);
		GtkHelper::gtk_dialog_add_button(
			GTK_DIALOG(result),
			button_text,
			response_id);
		button_text = va_arg(varargs, const gchar *);
	}

	va_end(varargs);
	return result;
}

gchar *gtk_file_chooser_get_current_folder(GtkFileChooser *chooser) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_file_chooser_get_current_folder(chooser);
}

gchar *gtk_file_chooser_get_filename(GtkFileChooser *chooser) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_file_chooser_get_filename(chooser);
}

GSList *gtk_file_chooser_get_filenames(GtkFileChooser *chooser) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_file_chooser_get_filenames(chooser);
}

GtkFileFilter *gtk_file_chooser_get_filter(GtkFileChooser *chooser) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_file_chooser_get_filter(chooser);
}

char *gtk_file_chooser_get_preview_filename(GtkFileChooser *chooser) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_file_chooser_get_preview_filename(chooser);
}

GType gtk_file_chooser_get_type(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_file_chooser_get_type();
}

void gtk_file_chooser_remove_filter(
	GtkFileChooser *chooser,
	GtkFileFilter *filter) {
	GtkHelper::Resolve();
	GtkHelper::gtk_file_chooser_remove_filter(chooser, filter);
}

gboolean gtk_file_chooser_select_filename(
	GtkFileChooser *chooser,
	const char *filename) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_file_chooser_select_filename(chooser, filename);
}

void gtk_file_chooser_set_action(
	GtkFileChooser *chooser,
	GtkFileChooserAction action) {
	GtkHelper::Resolve();
	GtkHelper::gtk_file_chooser_set_action(chooser, action);
}

void gtk_file_chooser_set_create_folders(
	GtkFileChooser *chooser,
	gboolean create_folders) {
	GtkHelper::Resolve();
	GtkHelper::gtk_file_chooser_set_create_folders(
		chooser,
		create_folders);
}

gboolean gtk_file_chooser_set_current_folder(
	GtkFileChooser *chooser,
	const gchar *filename) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_file_chooser_set_current_folder(chooser, filename);
}

void gtk_file_chooser_set_current_name(
	GtkFileChooser *chooser,
	const gchar *name) {
	GtkHelper::Resolve();
	GtkHelper::gtk_file_chooser_set_current_name(chooser, name);
}

void gtk_file_chooser_set_do_overwrite_confirmation(
	GtkFileChooser *chooser,
	gboolean do_overwrite_confirmation) {
	GtkHelper::Resolve();
	GtkHelper::gtk_file_chooser_set_do_overwrite_confirmation(
		chooser,
		do_overwrite_confirmation);
}

void gtk_file_chooser_set_filter(
	GtkFileChooser *chooser,
	GtkFileFilter *filter) {
	GtkHelper::Resolve();
	GtkHelper::gtk_file_chooser_set_filter(chooser, filter);
}

void gtk_file_chooser_set_local_only(
	GtkFileChooser *chooser,
	gboolean local_only) {
	GtkHelper::Resolve();
	GtkHelper::gtk_file_chooser_set_local_only(chooser, local_only);
}

void gtk_file_chooser_set_preview_widget(
	GtkFileChooser *chooser,
	GtkWidget *preview_widget) {
	GtkHelper::Resolve();
	GtkHelper::gtk_file_chooser_set_preview_widget(chooser, preview_widget);
}

void gtk_file_chooser_set_preview_widget_active(
	GtkFileChooser *chooser,
	gboolean active) {
	GtkHelper::Resolve();
	GtkHelper::gtk_file_chooser_set_preview_widget_active(chooser, active);
}

void gtk_file_chooser_set_select_multiple(
	GtkFileChooser *chooser,
	gboolean select_multiple) {
	GtkHelper::Resolve();
	GtkHelper::gtk_file_chooser_set_select_multiple(chooser, select_multiple);
}

void gtk_file_filter_add_pattern(
	GtkFileFilter *filter,
	const gchar *pattern) {
	GtkHelper::Resolve();
	GtkHelper::gtk_file_filter_add_pattern(filter, pattern);
}

GtkFileFilter *gtk_file_filter_new(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_file_filter_new();
}

void gtk_file_filter_set_name(
	GtkFileFilter *filter,
	const gchar *name) {
	GtkHelper::Resolve();
	GtkHelper::gtk_file_filter_set_name(filter, name);
}

GtkWidget* gtk_font_chooser_dialog_new(
	const gchar *title,
	GtkWindow *parent) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_font_chooser_dialog_new(title, parent);
}

gchar* gtk_font_chooser_get_font(GtkFontChooser *fontchooser) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_font_chooser_get_font(fontchooser);
}

GType gtk_font_chooser_get_type(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_font_chooser_get_type();
}

void gtk_font_chooser_set_font(
	GtkFontChooser *fontchooser,
	const gchar *fontname) {
	GtkHelper::Resolve();
	GtkHelper::gtk_font_chooser_set_font(fontchooser, fontname);
}

guint32 gtk_get_current_event_time(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_get_current_event_time();
}

GType gtk_image_get_type(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_image_get_type();
}

GtkWidget* gtk_image_new(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_image_new();
}

void gtk_image_set_from_pixbuf(GtkImage *image, GdkPixbuf *pixbuf) {
	GtkHelper::Resolve();
	GtkHelper::gtk_image_set_from_pixbuf(image, pixbuf);
}

void gtk_init(int *argc, char ***argv) {
	GtkHelper::Resolve();
	GtkHelper::gtk_init(argc, argv);
}

GType gtk_menu_get_type(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_menu_get_type();
}

GType gtk_menu_item_get_type(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_menu_item_get_type();
}

GtkWidget* gtk_menu_item_new(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_menu_item_new();
}

void gtk_menu_item_set_label(GtkMenuItem *menu_item, const gchar *label) {
	GtkHelper::Resolve();
	GtkHelper::gtk_menu_item_set_label(menu_item, label);
}

void gtk_menu_item_set_submenu(GtkMenuItem *menu_item, GtkWidget *submenu) {
	GtkHelper::Resolve();
	GtkHelper::gtk_menu_item_set_submenu(menu_item, submenu);
}

void gtk_menu_item_set_use_underline(
	GtkMenuItem *menu_item,
	gboolean setting) {
	GtkHelper::Resolve();
	GtkHelper::gtk_menu_item_set_use_underline(menu_item, setting);
}

GtkWidget* gtk_menu_new(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_menu_new();
}

void gtk_menu_popdown(GtkMenu *menu) {
	GtkHelper::Resolve();
	GtkHelper::gtk_menu_popdown(menu);
}

void gtk_menu_popup(
	GtkMenu *menu,
	GtkWidget *parent_menu_shell,
	GtkWidget *parent_menu_item,
	GtkMenuPositionFunc func,
	gpointer data,
	guint button,
	guint32 activate_time) {
	GtkHelper::Resolve();
	GtkHelper::gtk_menu_popup(
		menu,
		parent_menu_shell,
		parent_menu_item,
		func,
		data,
		button,
		activate_time);
}

GType gtk_menu_shell_get_type(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_menu_shell_get_type();
}
void gtk_menu_shell_insert(
	GtkMenuShell *menu_shell,
	GtkWidget *child,
	gint position) {
	GtkHelper::Resolve();
	GtkHelper::gtk_menu_shell_insert(menu_shell, child, position);
}

void gtk_menu_shell_select_item(
	GtkMenuShell *menu_shell,
	GtkWidget *menu_item) {
	GtkHelper::Resolve();
	GtkHelper::gtk_menu_shell_select_item(menu_shell, menu_item);
}

GtkWidget* gtk_separator_menu_item_new(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_separator_menu_item_new();
}

GtkSettings* gtk_settings_get_default(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_settings_get_default();
}

void gtk_widget_destroy(GtkWidget *widget) {
	GtkHelper::Resolve();
	GtkHelper::gtk_widget_destroy(widget);
}

gint gtk_widget_get_scale_factor(GtkWidget *widget) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_widget_get_scale_factor(widget);
}

GType gtk_widget_get_type(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_widget_get_type();
}

GdkWindow *gtk_widget_get_window(GtkWidget *widget) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_widget_get_window(widget);
}

void gtk_widget_hide(GtkWidget *widget) {
	GtkHelper::Resolve();
	GtkHelper::gtk_widget_hide(widget);
}

gboolean gtk_widget_hide_on_delete(GtkWidget *widget) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_widget_hide_on_delete(widget);
}

void gtk_widget_realize(GtkWidget *widget) {
	GtkHelper::Resolve();
	GtkHelper::gtk_widget_realize(widget);
}

void gtk_widget_set_sensitive(GtkWidget *widget, gboolean sensitive) {
	GtkHelper::Resolve();
	GtkHelper::gtk_widget_set_sensitive(widget, sensitive);
}

void gtk_widget_set_visible(GtkWidget *widget, gboolean visible) {
	GtkHelper::Resolve();
	GtkHelper::gtk_widget_set_visible(widget, visible);
}

void gtk_widget_show(GtkWidget *widget) {
	GtkHelper::Resolve();
	GtkHelper::gtk_widget_show(widget);
}

GType gtk_window_get_type(void) {
	GtkHelper::Resolve();
	return GtkHelper::gtk_window_get_type();
}

void gtk_window_set_title(GtkWindow *window, const gchar *title) {
	GtkHelper::Resolve();
	GtkHelper::gtk_window_set_title(window, title);
}

void pango_font_description_free(PangoFontDescription *desc) {
	GtkHelper::Resolve();
	GtkHelper::pango_font_description_free(desc);
}

PangoFontDescription *pango_font_description_from_string(const char *str) {
	GtkHelper::Resolve();
	return GtkHelper::pango_font_description_from_string(str);
}

const char *pango_font_description_get_family(
	const PangoFontDescription *desc) {
	GtkHelper::Resolve();
	return GtkHelper::pango_font_description_get_family(desc);
}

gint pango_font_description_get_size(const PangoFontDescription *desc) {
	GtkHelper::Resolve();
	return GtkHelper::pango_font_description_get_size(desc);
}

PangoStyle pango_font_description_get_style(
	const PangoFontDescription *desc) {
	GtkHelper::Resolve();
	return GtkHelper::pango_font_description_get_style(desc);
}

PangoWeight pango_font_description_get_weight(
	const PangoFontDescription *desc) {
	GtkHelper::Resolve();
	return GtkHelper::pango_font_description_get_weight(desc);
}

PangoFontDescription *pango_font_description_new(void) {
	GtkHelper::Resolve();
	return GtkHelper::pango_font_description_new();
}

void pango_font_description_set_family(
	PangoFontDescription *desc,
	const char *family) {
	GtkHelper::Resolve();
	GtkHelper::pango_font_description_set_family(desc, family);
}

void pango_font_description_set_size(
	PangoFontDescription *desc,
	gint size) {
	GtkHelper::Resolve();
	GtkHelper::pango_font_description_set_size(desc, size);
}

void pango_font_description_set_style(
	PangoFontDescription *desc,
	PangoStyle style) {
	GtkHelper::Resolve();
	GtkHelper::pango_font_description_set_style(desc, style);
}

void pango_font_description_set_weight(
	PangoFontDescription *desc,
	PangoWeight weight) {
	GtkHelper::Resolve();
	GtkHelper::pango_font_description_set_weight(desc, weight);
}

char *pango_font_description_to_string(const PangoFontDescription *desc) {
	GtkHelper::Resolve();
	return GtkHelper::pango_font_description_to_string(desc);
}

GType pango_font_face_get_type(void) {
	GtkHelper::Resolve();
	return GtkHelper::pango_font_face_get_type();
}

GType pango_font_family_get_type(void) {
	GtkHelper::Resolve();
	return GtkHelper::pango_font_family_get_type();
}
