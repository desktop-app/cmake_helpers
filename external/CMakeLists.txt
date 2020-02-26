# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

macro(add_checked_subdirectory name)
    if (NOT DEFINED desktop_app_skip_libs
        OR NOT ${name} IN_LIST desktop_app_skip_libs)
        add_subdirectory(${name})
    endif()
endmacro()

add_checked_subdirectory(auto_updates)
add_checked_subdirectory(crash_reports)
if (LINUX AND NOT DESKTOP_APP_DISABLE_DBUS_INTEGRATION)
    add_checked_subdirectory(dbusmenu_qt)
endif()
add_checked_subdirectory(expected)
add_checked_subdirectory(ffmpeg)
add_checked_subdirectory(gsl)
if (add_hunspell_library)
	add_checked_subdirectory(hunspell)
endif()
add_checked_subdirectory(iconv)
add_checked_subdirectory(lz4)
add_checked_subdirectory(minizip)
add_checked_subdirectory(openal)
add_checked_subdirectory(openssl)
add_checked_subdirectory(opus)
add_checked_subdirectory(qt)
add_checked_subdirectory(qr_code_generator)
add_checked_subdirectory(ranges)
add_checked_subdirectory(rlottie)
if (APPLE)
    add_checked_subdirectory(sp_media_key_tap)
endif()
if (LINUX AND NOT DESKTOP_APP_DISABLE_DBUS_INTEGRATION)
    add_checked_subdirectory(statusnotifieritem)
endif()
add_checked_subdirectory(ton)
add_checked_subdirectory(variant)
add_checked_subdirectory(xxhash)
add_checked_subdirectory(zlib)
