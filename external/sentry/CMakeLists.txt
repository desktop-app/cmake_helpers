# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_sentry STATIC IMPORTED GLOBAL)
add_library(desktop-app::external_sentry ALIAS external_sentry)

set(sentry_lib_loc ${libs_loc}/sentry-native/out)

include(sentry_helper.cmake)
include(crashpad_client.cmake)

if (WIN32)
    target_include_directories(external_sentry SYSTEM
    INTERFACE
        ${libs_loc}/sentry-native/include
    )
    set_target_properties(external_sentry PROPERTIES
        IMPORTED_LOCATION "${sentry_lib_loc}/Release/sentry.lib"
        IMPORTED_LOCATION_DEBUG "${sentry_lib_loc}/Debug/sentry.lib"
    )

    target_link_libraries(external_sentry
    INTERFACE
        desktop-app::sentry_crashpad_client
        desktop-app::sentry_helper
    )
elseif (APPLE)
    target_include_directories(external_sentry SYSTEM
    INTERFACE
        ${libs_loc}/sentry-native/include
    )
    set_target_properties(external_sentry PROPERTIES
        IMPORTED_LOCATION "${sentry_lib_loc}/Release/libsentry.a"
        IMPORTED_LOCATION_DEBUG "${sentry_lib_loc}/Debug/libsentry.a"
    )
else()
    find_library(DESKTOP_APP_SENTRY_LIBRARIES libsentry.a REQUIRED)
    set_target_properties(external_sentry PROPERTIES
        IMPORTED_LOCATION "${DESKTOP_APP_SENTRY_LIBRARIES}"
    )
endif()
