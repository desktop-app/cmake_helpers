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
    target_include_directories(common_options
    INTERFACE
        /usr/local/macos/include
    )
endif()

target_compile_options(common_options
INTERFACE
    -pipe
    -g
    -Wall
    -Werror
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
)

target_link_frameworks(common_options
INTERFACE
    Cocoa
    CoreFoundation
    CoreServices
    CoreText
    CoreGraphics
    CoreMedia
    IOSurface
    Metal
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
    QuartzCore
    AppKit
    CoreWLAN
    IOKit
)
