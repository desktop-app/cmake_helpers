# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (build_osx)
    target_compile_definitions(common_options INTERFACE OS_OSX)
else()
    if (build_macstore)
        target_compile_definitions(common_options
        INTERFACE
            OS_MAC_STORE
            MAC_USE_BREAKPAD
        )
    endif()
    if (NOT DESKTOP_APP_USE_PACKAGED)
        target_include_directories(common_options
        INTERFACE
            /usr/local/macos/include
        )
    endif()
endif()

target_compile_options(common_options
INTERFACE
    -pipe
    -Wall
    -W
    -fPIE
    -Wno-deprecated-declarations # temp for range-v3
    -Wno-unused-variable
    -Wno-unused-parameter
    -Wno-unused-function
    -Wno-switch
    -Wno-comment
    -Wno-missing-field-initializers
    -Wno-sign-compare
    -Wno-unknown-attributes
    -Wno-pragma-system-header-outside-header
    -Wno-range-loop-analysis
)

if (DESKTOP_APP_SPECIAL_TARGET)
    target_compile_options(common_options
    INTERFACE
        -g
        -Werror
    )
endif()

target_link_frameworks(common_options
INTERFACE
    Cocoa
    CoreFoundation
    CoreServices
    CoreText
    CoreGraphics
    CoreMedia
    OpenGL
    AudioUnit
    ApplicationServices
    Foundation
    AGL
    Security
    SystemConfiguration
    Carbon
    AudioToolbox
    VideoToolbox
    VideoDecodeAcceleration
    AVFoundation
    CoreAudio
    CoreVideo
    QuartzCore
    AppKit
    CoreWLAN
    IOKit
    GSS
)

if (NOT build_osx)
    target_link_frameworks(common_options
    INTERFACE
        IOSurface
        Metal
    )
endif()
