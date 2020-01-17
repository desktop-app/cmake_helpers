# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_opus INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_opus ALIAS external_opus)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(OPUS opus)

    target_include_directories(external_opus INTERFACE ${OPUS_INCLUDE_DIRS})
else()
    target_include_directories(external_opus SYSTEM
    INTERFACE
        ${libs_loc}/opus/include
    )
endif()

if (DESKTOP_APP_USE_PACKAGED)
    target_link_libraries(external_opus INTERFACE ${OPUS_LIBRARIES})
elseif (WIN32)
    set(opus_lib_loc ${libs_loc}/opus/win32/VS2015/Win32/$<IF:$<CONFIG:Debug>,Debug,Release>)

    target_link_libraries(external_opus
    INTERFACE
        ${opus_lib_loc}/opus.lib
    )
elseif (APPLE AND NOT build_osx)
    target_include_directories(external_opus
    INTERFACE
        /usr/local/macos/include
    )
    target_link_libraries(external_opus
    INTERFACE
        /usr/local/macos/lib/libopus.a
    )
elseif (APPLE)
    target_include_directories(external_opus
    INTERFACE
        /usr/local/include
    )
    target_link_libraries(external_opus
    INTERFACE
        /usr/local/lib/libopus.a
    )
else()
    target_link_static_libraries(external_opus
    INTERFACE
        opus
    )
endif()
