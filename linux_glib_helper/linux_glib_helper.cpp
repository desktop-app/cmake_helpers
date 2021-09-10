// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include <dlfcn.h>
#include <glib.h>
#include <glib-object.h>
#include <memory>
#include <iostream>

#define LOAD_SYMBOL(handle, func) LoadSymbol(handle, #func, func)

namespace GlibHelper {
namespace {

void (*g_free)(gpointer mem);
void (*g_log_default_handler)(
	const gchar* log_domain,
	GLogLevelFlags log_level,
	const gchar* message,
	gpointer unused_data);
guint (*g_log_set_handler)(
	const gchar* log_domain,
	GLogLevelFlags log_levels,
	GLogFunc log_func,
	gpointer user_data);
GMainContext* (*g_main_context_default)(void);
gboolean (*g_main_context_iteration)(GMainContext* context, gboolean may_block);
GMainContext* (*g_main_context_new)(void);
gboolean (*g_main_context_pending)(GMainContext* context);
void (*g_main_context_pop_thread_default)(GMainContext* context);
void (*g_main_context_push_thread_default)(GMainContext* context);
GMainContext* (*g_main_context_ref)(GMainContext* context);
void (*g_main_context_unref)(GMainContext* context);
void (*g_main_context_wakeup)(GMainContext* context);
void (*g_return_if_fail_warning)(
	const char* log_domain,
	const char* pretty_function,
	const char* expression);
void (*g_slist_free)(GSList* list);
void (*g_source_add_poll)(GSource* source, GPollFD* fd);
guint (*g_source_attach)(GSource* source, GMainContext* context);
void (*g_source_destroy)(GSource* source);
GSource* (*g_source_new)(GSourceFuncs* source_funcs, guint struct_size);
void (*g_source_remove_poll)(GSource* source, GPollFD* fd);
void (*g_source_set_can_recurse)(GSource* source, gboolean can_recurse);
void (*g_source_unref)(GSource* source);
int (*g_strcmp0)(const char* str1, const char* str2);

GParamSpec* (*g_object_class_find_property)(
	GObjectClass* oclass,
	const gchar* property_name);
void (*g_object_get_valist)(
	GObject* object,
	const gchar* first_property_name,
	va_list var_args);
GObject* (*g_object_new_with_properties)(
	GType object_type,
	guint n_properties,
	const char** names,
	const GValue* values);
GObject* (*g_object_new_valist)(
	GType object_type,
	const gchar* first_property_name,
	va_list var_args);
void (*g_object_set_valist)(
	GObject* object,
	const gchar* first_property_name,
	va_list var_args);
void (*g_object_unref)(gpointer object);
gulong (*g_signal_connect_data)(
	gpointer instance,
	const gchar* detailed_signal,
	GCallback c_handler,
	gpointer data,
	GClosureNotify destroy_data,
	GConnectFlags connect_flags);
GTypeInstance* (*g_type_check_instance_cast)(
	GTypeInstance* instance,
	GType iface_type);
gboolean (*g_type_check_instance_is_a)(
	GTypeInstance* instance,
	GType iface_type);
gboolean (*g_type_check_instance_is_fundamentally_a)(
	GTypeInstance* instance,
	GType fundamental_type);
void (*g_type_ensure)(GType type);

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
		return LoadLibrary(lib, "libglib-2.0.so.0")
			&& LOAD_SYMBOL(lib, g_free)
			&& LOAD_SYMBOL(lib, g_log_default_handler)
			&& LOAD_SYMBOL(lib, g_log_set_handler)
			&& LOAD_SYMBOL(lib, g_main_context_default)
			&& LOAD_SYMBOL(lib, g_main_context_iteration)
			&& LOAD_SYMBOL(lib, g_main_context_new)
			&& LOAD_SYMBOL(lib, g_main_context_pending)
			&& LOAD_SYMBOL(lib, g_main_context_pop_thread_default)
			&& LOAD_SYMBOL(lib, g_main_context_push_thread_default)
			&& LOAD_SYMBOL(lib, g_main_context_ref)
			&& LOAD_SYMBOL(lib, g_main_context_unref)
			&& LOAD_SYMBOL(lib, g_main_context_wakeup)
			&& LOAD_SYMBOL(lib, g_return_if_fail_warning)
			&& LOAD_SYMBOL(lib, g_slist_free)
			&& LOAD_SYMBOL(lib, g_source_add_poll)
			&& LOAD_SYMBOL(lib, g_source_attach)
			&& LOAD_SYMBOL(lib, g_source_destroy)
			&& LOAD_SYMBOL(lib, g_source_new)
			&& LOAD_SYMBOL(lib, g_source_remove_poll)
			&& LOAD_SYMBOL(lib, g_source_set_can_recurse)
			&& LOAD_SYMBOL(lib, g_source_unref)
			&& LOAD_SYMBOL(lib, g_strcmp0)
			&& LoadLibrary(lib, "libgobject-2.0.so.0")
			&& LOAD_SYMBOL(lib, g_object_class_find_property)
			&& LOAD_SYMBOL(lib, g_object_get_valist)
			&& LOAD_SYMBOL(lib, g_object_new_valist)
			&& LOAD_SYMBOL(lib, g_object_new_with_properties)
			&& LOAD_SYMBOL(lib, g_object_set_valist)
			&& LOAD_SYMBOL(lib, g_object_unref)
			&& LOAD_SYMBOL(lib, g_signal_connect_data)
			&& LOAD_SYMBOL(lib, g_type_check_instance_cast)
			&& LOAD_SYMBOL(lib, g_type_check_instance_is_a)
			&& LOAD_SYMBOL(lib, g_type_check_instance_is_fundamentally_a)
			&& LOAD_SYMBOL(lib, g_type_ensure);
	}();
	return loaded;
}

} // namespace
} // namespace GlibHelper

void g_free(gpointer mem) {
	GlibHelper::Resolve();
	GlibHelper::g_free(mem);
}

void g_log_default_handler(
	const gchar* log_domain,
	GLogLevelFlags log_level,
	const gchar* message,
	gpointer unused_data) {
	GlibHelper::Resolve();
	GlibHelper::g_log_default_handler(
		log_domain,
		log_level,
		message,
		unused_data);
}

guint g_log_set_handler(
	const gchar* log_domain,
	GLogLevelFlags log_levels,
	GLogFunc log_func,
	gpointer user_data) {
	GlibHelper::Resolve();
	return GlibHelper::g_log_set_handler(
		log_domain,
		log_levels,
		log_func,
		user_data);
}

GMainContext* g_main_context_default(void) {
	GlibHelper::Resolve();
	return GlibHelper::g_main_context_default();
}

gboolean g_main_context_iteration(GMainContext* context, gboolean may_block) {
	GlibHelper::Resolve();
	return GlibHelper::g_main_context_iteration(context, may_block);
}

GMainContext* g_main_context_new(void) {
	GlibHelper::Resolve();
	return GlibHelper::g_main_context_new();
}

gboolean g_main_context_pending(GMainContext* context) {
	GlibHelper::Resolve();
	return GlibHelper::g_main_context_pending(context);
}

void g_main_context_pop_thread_default(GMainContext* context) {
	GlibHelper::Resolve();
	GlibHelper::g_main_context_pop_thread_default(context);
}

void g_main_context_push_thread_default(GMainContext* context) {
	GlibHelper::Resolve();
	GlibHelper::g_main_context_push_thread_default(context);
}

GMainContext* g_main_context_ref(GMainContext* context) {
	GlibHelper::Resolve();
	return GlibHelper::g_main_context_ref(context);
}

void g_main_context_unref(GMainContext* context) {
	GlibHelper::Resolve();
	GlibHelper::g_main_context_unref(context);
}

void g_main_context_wakeup(GMainContext* context) {
	GlibHelper::Resolve();
	GlibHelper::g_main_context_wakeup(context);
}

void g_return_if_fail_warning(
	const char* log_domain,
	const char* pretty_function,
	const char* expression) {
	GlibHelper::Resolve();
	GlibHelper::g_return_if_fail_warning(
		log_domain,
		pretty_function,
		expression);
}

void g_slist_free(GSList* list) {
	GlibHelper::Resolve();
	GlibHelper::g_slist_free(list);
}

void g_source_add_poll(GSource* source, GPollFD* fd) {
	GlibHelper::Resolve();
	GlibHelper::g_source_add_poll(source, fd);
}

guint g_source_attach(GSource* source, GMainContext* context) {
	GlibHelper::Resolve();
	return GlibHelper::g_source_attach(source, context);
}

void g_source_destroy(GSource* source) {
	GlibHelper::Resolve();
	GlibHelper::g_source_destroy(source);
}

GSource* g_source_new(GSourceFuncs* source_funcs, guint struct_size) {
	GlibHelper::Resolve();
	return GlibHelper::g_source_new(source_funcs, struct_size);
}

void g_source_remove_poll(GSource* source, GPollFD* fd) {
	GlibHelper::Resolve();
	GlibHelper::g_source_remove_poll(source, fd);
}

void g_source_set_can_recurse(GSource* source, gboolean can_recurse) {
	GlibHelper::Resolve();
	GlibHelper::g_source_set_can_recurse(source, can_recurse);
}

void g_source_unref(GSource* source) {
	GlibHelper::Resolve();
	GlibHelper::g_source_unref(source);
}

int g_strcmp0(const char* str1, const char* str2) {
	GlibHelper::Resolve();
	return GlibHelper::g_strcmp0(str1, str2);
}

GParamSpec* g_object_class_find_property(
	GObjectClass* oclass,
	const gchar* property_name) {
	GlibHelper::Resolve();
	return GlibHelper::g_object_class_find_property(
		oclass,
		property_name);
}

void g_object_get(
	gpointer _object,
	const gchar *first_property_name,
	...) {
	GlibHelper::Resolve();

	GObject *object = (GObject*) _object;
	va_list var_args;
	
	g_return_if_fail(G_IS_OBJECT(object));
	
	va_start(var_args, first_property_name);
	GlibHelper::g_object_get_valist(object, first_property_name, var_args);
	va_end(var_args);
}

gpointer g_object_new(
	GType object_type,
	const gchar *first_property_name,
	...) {
	GlibHelper::Resolve();

	GObject *object;
	va_list var_args;
	
	/* short circuit for calls supplying no properties */
	if (!first_property_name)
		return GlibHelper::g_object_new_with_properties(object_type, 0, NULL, NULL);

	va_start(var_args, first_property_name);
	object = GlibHelper::g_object_new_valist(object_type, first_property_name, var_args);
	va_end(var_args);
	
	return object;
}

void g_object_set(
	gpointer _object,
	const gchar *first_property_name,
	...) {
	GlibHelper::Resolve();

	GObject *object = (GObject*) _object;
	va_list var_args;
	
	g_return_if_fail(G_IS_OBJECT(object));
	
	va_start(var_args, first_property_name);
	GlibHelper::g_object_set_valist(object, first_property_name, var_args);
	va_end(var_args);
}

void g_object_unref(gpointer object) {
	GlibHelper::Resolve();
	GlibHelper::g_object_unref(object);
}

gulong g_signal_connect_data(
	gpointer instance,
	const gchar* detailed_signal,
	GCallback c_handler,
	gpointer data,
	GClosureNotify destroy_data,
	GConnectFlags connect_flags) {
	GlibHelper::Resolve();
	return GlibHelper::g_signal_connect_data(
		instance,
		detailed_signal,
		c_handler,
		data,
		destroy_data,
		connect_flags);
}

GTypeInstance* g_type_check_instance_cast(
	GTypeInstance* instance,
	GType iface_type) {
	GlibHelper::Resolve();
	return GlibHelper::g_type_check_instance_cast(instance, iface_type);
}

gboolean g_type_check_instance_is_a(
	GTypeInstance* instance,
	GType iface_type) {
	GlibHelper::Resolve();
	return GlibHelper::g_type_check_instance_is_a(instance, iface_type);
}

gboolean g_type_check_instance_is_fundamentally_a(
	GTypeInstance* instance,
	GType fundamental_type) {
	GlibHelper::Resolve();
	return GlibHelper::g_type_check_instance_is_fundamentally_a(
		instance,
		fundamental_type);
}

void g_type_ensure(GType type) {
	GlibHelper::Resolve();
	GlibHelper::g_type_ensure(type);
}
