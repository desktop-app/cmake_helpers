# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_webrtc INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_webrtc ALIAS external_webrtc)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(tg_owt REQUIRED)
    target_link_libraries(external_webrtc INTERFACE tg_owt::tg_owt)
else()
    set(webrtc_loc ${libs_loc}/tg_owt/src)
    set(webrtc_build_loc ${libs_loc}/tg_owt/out/$<CONFIG>)

    target_compile_definitions(external_webrtc
    INTERFACE
        WEBRTC_ENABLE_PROTOBUF=0
        WEBRTC_INCLUDE_INTERNAL_AUDIO_DEVICE
        RTC_ENABLE_VP9
        HAVE_SCTP
        WEBRTC_USE_H264
        WEBRTC_LIBRARY_IMPL
        WEBRTC_NON_STATIC_TRACE_EVENT_HANDLERS=1
        ABSL_ALLOCATOR_NOTHROW=1
    )

    if (WIN32)
        set(webrtc_lib_prefix "")
        set(webrtc_lib_suffix .lib)

        target_compile_definitions(external_webrtc
        INTERFACE
            WEBRTC_WIN
        )
    else()
        set(webrtc_lib_prefix lib)
        set(webrtc_lib_suffix .a)

        if (APPLE)
            target_compile_definitions(external_webrtc
            INTERFACE
                WEBRTC_MAC
            )
        else()
            target_compile_definitions(external_webrtc
            INTERFACE
                WEBRTC_LINUX
            )
        endif()

        target_compile_definitions(external_webrtc
        INTERFACE
            WEBRTC_POSIX
        )
    endif()

    target_include_directories(external_webrtc
    INTERFACE
        ${webrtc_loc}
        ${webrtc_loc}/third_party/abseil-cpp
    )

    set(webrtc_libs
        ${webrtc_lib_prefix}tg_owt
    )
    if (APPLE)
        target_include_directories(external_webrtc
        INTERFACE
            ${webrtc_loc}/sdk/objc
            ${webrtc_loc}/sdk/objc/base
            ${webrtc_loc}/sdk/objc/components/video_codec
            ${webrtc_loc}/third_party/libyuv/include
        )
    endif()
    foreach (lib ${webrtc_libs})
        list(APPEND webrtc_libs_list "${webrtc_build_loc}/${lib}${webrtc_lib_suffix}")
    endforeach()

    if (WIN32)
        target_link_libraries(external_webrtc
        INTERFACE
            Secur32.lib # Required for rtc_base/http_common.cc
            dmoguids.lib # Required for the built-in WASAPI AEC, see modules/audio_device/BUILD.gn
            wmcodecdspuuid.lib
            amstrmid.lib
            msdmo.lib
        )
    elseif (APPLE)
        target_link_libraries(external_webrtc
        INTERFACE
            -ObjC
        )
    endif()

    target_link_libraries(external_webrtc
    INTERFACE
        ${webrtc_libs_list}
        desktop-app::external_openssl
        desktop-app::external_jpeg
        desktop-app::external_opus
    )
endif()
