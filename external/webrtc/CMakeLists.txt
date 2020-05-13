# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_webrtc INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_webrtc ALIAS external_webrtc)

set(webrtc_loc ${libs_loc}/webrtc/src)
set(webrtc_build_loc ${webrtc_loc}/out/$<CONFIG>/obj)
if (WIN32)
    set(webrtc_lib_prefix $<CONFIG>/)
    set(webrtc_lib_suffix .lib)

    target_compile_definitions(external_webrtc
    INTERFACE
        WEBRTC_WIN
    )
else()
    set(webrtc_lib_prefix lib)
    set(webrtc_lib_suffix .a)

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
    webrtc
)
foreach (lib ${webrtc_libs})
    list(APPEND webrtc_libs_list "${webrtc_build_loc}/${lib}${webrtc_lib_suffix}")
endforeach()

target_link_libraries(external_webrtc
INTERFACE
    ${webrtc_libs_list}
)
