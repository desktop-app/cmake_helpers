# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_ffmpeg INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_ffmpeg ALIAS external_ffmpeg)

if (DESKTOP_APP_USE_PACKAGED OR LINUX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(DESKTOP_APP_FFMPEG REQUIRED IMPORTED_TARGET
        libavcodec
        libavfilter
        libavformat
        libavutil
        libswresample
        libswscale
    )
    target_link_libraries(external_ffmpeg INTERFACE PkgConfig::DESKTOP_APP_FFMPEG)
    return()
endif()

set(ffmpeg_lib_loc ${libs_loc}/ffmpeg)
if (WIN32)
    set(ffmpeg_lib_prefix "")
    set(ffmpeg_lib_suffix .lib)
else()
    set(ffmpeg_lib_prefix lib)
    set(ffmpeg_lib_suffix .a)
endif()

target_include_directories(external_ffmpeg SYSTEM
INTERFACE
    ${ffmpeg_lib_loc}
)

target_link_libraries(external_ffmpeg
INTERFACE
    ${ffmpeg_lib_loc}/libavfilter/${ffmpeg_lib_prefix}avfilter${ffmpeg_lib_suffix}
    ${ffmpeg_lib_loc}/libavformat/${ffmpeg_lib_prefix}avformat${ffmpeg_lib_suffix}
    ${ffmpeg_lib_loc}/libavcodec/${ffmpeg_lib_prefix}avcodec${ffmpeg_lib_suffix}
    ${ffmpeg_lib_loc}/libswresample/${ffmpeg_lib_prefix}swresample${ffmpeg_lib_suffix}
    ${ffmpeg_lib_loc}/libswscale/${ffmpeg_lib_prefix}swscale${ffmpeg_lib_suffix}
    ${ffmpeg_lib_loc}/libavutil/${ffmpeg_lib_prefix}avutil${ffmpeg_lib_suffix}
    $<LINK_ONLY:desktop-app::external_openh264>
    $<TARGET_FILE:desktop-app::external_openh264>
    $<LINK_ONLY:desktop-app::external_opus>
    $<TARGET_FILE:desktop-app::external_opus>
    $<LINK_ONLY:desktop-app::external_vpx>
    $<TARGET_FILE:desktop-app::external_vpx>
)

if (APPLE)
    target_link_libraries(external_ffmpeg INTERFACE bz2)
endif()
