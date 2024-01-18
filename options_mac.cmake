# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (build_macstore)
    target_compile_definitions(common_options
    INTERFACE
        OS_MAC_STORE
        MAC_USE_BREAKPAD
    )
endif()
if (NOT DESKTOP_APP_USE_PACKAGED)
    target_include_directories(common_options SYSTEM
    INTERFACE
        ${libs_loc}/local/include
    )
endif()

target_compile_options(common_options
INTERFACE
    -pipe
    -Wall
    -Wextra
    -fPIE
    $<$<COMPILE_LANGUAGE:OBJC,OBJCXX>:-fobjc-weak>
    -fvisibility-inlines-hidden
    -fvisibility=hidden
    -Wno-unused-variable
    -Wno-unused-parameter
    -Wno-unused-function
    -Wno-switch
    -Wno-comment
    -Wno-missing-field-initializers
    -Wno-sign-compare
    -Wno-unknown-attributes
    -Wno-pragma-system-header-outside-header
    -Wno-shorten-64-to-32
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
    CoreMediaIO
    QuartzCore
    AppKit
    CoreWLAN
    WebKit
    IOKit
    GSS
    MediaPlayer
    IOSurface
    Metal
)
