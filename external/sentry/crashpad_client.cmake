# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(sentry_crashpad_client INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::sentry_crashpad_client ALIAS sentry_crashpad_client)


if (WIN32)
    set(crashpad_build_loc "${sentry_lib_loc}/$<IF:$<CONFIG:Debug>,Debug,Release>/crashpad_build")
    target_link_libraries(sentry_crashpad_client
    INTERFACE
        ${crashpad_build_loc}/client/crashpad_client.lib
        ${crashpad_build_loc}/util/crashpad_util.lib
        ${crashpad_build_loc}/third_party/mini_chromium/mini_chromium.lib
    )
elseif (APPLE)
    message(FATAL_ERROR "not implemented")
else()
    message(FATAL_ERROR "not implemented")
endif()
