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
else()
    target_include_directories(external_ffmpeg SYSTEM
    INTERFACE
        ${libs_loc}/ffmpeg
    )

    set(ffmpeg_lib_loc ${libs_loc}/ffmpeg)

    target_link_libraries(external_ffmpeg
    INTERFACE
        ${ffmpeg_lib_loc}/libavformat/libavformat.a
        ${ffmpeg_lib_loc}/libavcodec/libavcodec.a
        ${ffmpeg_lib_loc}/libswresample/libswresample.a
        ${ffmpeg_lib_loc}/libswscale/libswscale.a
        ${ffmpeg_lib_loc}/libavutil/libavutil.a
        $<TARGET_FILE:desktop-app::external_opus>
    )
    if (LINUX)
        target_link_static_libraries(external_ffmpeg
        INTERFACE
            lzma
            va-x11
            va-drm
            va
            vdpau
            Xv
            Xext
            Xfixes
            drm
        )
        target_link_libraries(external_ffmpeg
        INTERFACE
            X11
            pthread
        )
    endif()
endif()
