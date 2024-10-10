# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(common_options INTERFACE)
add_library(desktop-app::common_options ALIAS common_options)

target_compile_definitions(common_options
INTERFACE
    $<$<CONFIG:Debug>:_DEBUG>
    QT_NO_KEYWORDS
    QT_NO_CAST_FROM_BYTEARRAY
    QT_DEPRECATED_WARNINGS_SINCE=0x051500
)

if (DESKTOP_APP_DISABLE_X11_INTEGRATION)
    target_compile_definitions(common_options
    INTERFACE
        DESKTOP_APP_DISABLE_X11_INTEGRATION
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
