# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_jpeg INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_jpeg ALIAS external_jpeg)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(JPEG)
    target_link_libraries(external_jpeg INTERFACE JPEG::JPEG)
elseif (WIN32)
    target_include_directories(external_jpeg INTERFACE ${libs_loc}/mozjpeg)
    target_link_libraries(external_jpeg INTERFACE ${libs_loc}/mozjpeg/$<IF:$<CONFIG:Debug>,Debug,Release>/jpeg-static.lib)
elseif (APPLE AND NOT build_osx)
    target_link_libraries(external_jpeg INTERFACE /usr/local/macos/lib/libjpeg.a)
elseif (APPLE)
    target_link_libraries(external_jpeg INTERFACE /usr/local/lib/libjpeg.a)
else()
    target_link_static_libraries(external_jpeg INTERFACE jpeg)
endif()
