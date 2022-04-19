# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_ffmpeg INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_ffmpeg ALIAS external_ffmpeg)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(AVCODEC REQUIRED IMPORTED_TARGET libavcodec)
    pkg_check_modules(AVFORMAT REQUIRED IMPORTED_TARGET libavformat)
    pkg_check_modules(AVUTIL REQUIRED IMPORTED_TARGET libavutil)
    pkg_check_modules(SWSCALE REQUIRED IMPORTED_TARGET libswscale)
    pkg_check_modules(SWRESAMPLE REQUIRED IMPORTED_TARGET libswresample)

    target_link_libraries(external_ffmpeg
    INTERFACE
        PkgConfig::AVCODEC
        PkgConfig::AVFORMAT
        PkgConfig::AVUTIL
        PkgConfig::SWSCALE
        PkgConfig::SWRESAMPLE
    )

    return()
endif()

set(ffmpeg_lib_list)

if (LINUX)
    list(APPEND ffmpeg_lib_list
        -lavformat
        -lavcodec
        -lswresample
        -lswscale
        -lavutil
    )
else()
    set(ffmpeg_lib_loc ${libs_loc}/ffmpeg)

    target_include_directories(external_ffmpeg SYSTEM
    INTERFACE
        ${ffmpeg_lib_loc}
    )

    list(APPEND ffmpeg_lib_list
        ${ffmpeg_lib_loc}/libavformat/libavformat.a
        ${ffmpeg_lib_loc}/libavcodec/libavcodec.a
        ${ffmpeg_lib_loc}/libswresample/libswresample.a
        ${ffmpeg_lib_loc}/libswscale/libswscale.a
        ${ffmpeg_lib_loc}/libavutil/libavutil.a
    )
endif()

list(APPEND ffmpeg_lib_list
    $<TARGET_FILE:desktop-app::external_opus>
    $<TARGET_FILE:desktop-app::external_vpx>
)

if (APPLE)
    list(APPEND ffmpeg_lib_list bz2)
elseif (LINUX)
    list(APPEND ffmpeg_lib_list -llzma)
endif()

# Workaround cmake's random order on Linux...
if (LINUX)
    list(JOIN ffmpeg_lib_list , ffmpeg_lib_link)
    target_link_libraries(external_ffmpeg
    INTERFACE
        -Wl,--push-state,-Bstatic,${ffmpeg_lib_link},--pop-state
    )
else()
    target_link_libraries(external_ffmpeg
    INTERFACE
        ${ffmpeg_lib_list}
    )
endif()

target_link_libraries(external_ffmpeg
INTERFACE
    $<LINK_ONLY:desktop-app::external_opus>
    $<LINK_ONLY:desktop-app::external_vpx>
)
