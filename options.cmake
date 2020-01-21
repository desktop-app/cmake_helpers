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

if (DESKTOP_APP_USE_PACKAGED)
    target_compile_definitions(common_options
    INTERFACE
        DESKTOP_APP_USE_PACKAGED
    )
endif()

if (DESKTOP_APP_USE_PACKAGED_FONTS)
    target_compile_definitions(common_options
    INTERFACE
        DESKTOP_APP_USE_PACKAGED_FONTS
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
