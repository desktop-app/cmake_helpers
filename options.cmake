# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(common_options INTERFACE)
add_library(desktop-app::common_options ALIAS common_options)

target_compile_definitions(common_options
INTERFACE
    $<IF:$<CONFIG:Debug>,_DEBUG,NDEBUG>
)

if (DESKTOP_APP_DISABLE_CRASH_REPORTS)
    target_compile_definitions(common_options
    INTERFACE
        DESKTOP_APP_DISABLE_CRASH_REPORTS
    )
endif()

if (DESKTOP_APP_DISABLE_DBUS_INTEGRATION)
    target_compile_definitions(common_options
    INTERFACE
        DESKTOP_APP_DISABLE_DBUS_INTEGRATION
    )
endif()

if (DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION)
    target_compile_definitions(common_options
    INTERFACE
        DESKTOP_APP_DISABLE_WAYLAND_INTEGRATION
    )
endif()

if (DESKTOP_APP_DISABLE_WEBRTC_INTEGRATION)
    target_compile_definitions(common_options
    INTERFACE
        DESKTOP_APP_DISABLE_WEBRTC_INTEGRATION
    )
endif()

if (DESKTOP_APP_USE_PACKAGED)
    target_compile_definitions(common_options
    INTERFACE
        DESKTOP_APP_USE_PACKAGED
    )
endif()

if (DESKTOP_APP_USE_PACKAGED_LAZY)
    target_compile_definitions(common_options
    INTERFACE
        DESKTOP_APP_USE_PACKAGED_LAZY
    )
endif()

if (DESKTOP_APP_USE_PACKAGED_LAZY_PLATFORMTHEMES)
    target_compile_definitions(common_options
    INTERFACE
        DESKTOP_APP_USE_PACKAGED_LAZY_PLATFORMTHEMES
    )
endif()

if (DESKTOP_APP_USE_PACKAGED_FONTS)
    target_compile_definitions(common_options
    INTERFACE
        DESKTOP_APP_USE_PACKAGED_FONTS
    )
endif()

if (rlottie_FOUND OR RLOTTIE_FOUND)
    target_compile_definitions(common_options
    INTERFACE
        DESKTOP_APP_USE_PACKAGED_RLOTTIE
    )
endif()

if (NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "")
    target_compile_definitions(common_options
    INTERFACE
        DESKTOP_APP_SPECIAL_TARGET=${DESKTOP_APP_SPECIAL_TARGET}
    )
endif()

if (WIN32)
    include(cmake/options_win.cmake)
elseif (APPLE)
    include(cmake/options_mac.cmake)
elseif (LINUX)
    include(cmake/options_linux.cmake)
else()
    message(FATAL_ERROR "Unknown platform type")
endif()
