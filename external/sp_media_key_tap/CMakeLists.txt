# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (EXISTS ${third_party_loc}/SPMediaKeyTap)
    add_library(external_sp_media_key_tap STATIC)
    add_library(desktop-app::external_sp_media_key_tap ALIAS external_sp_media_key_tap)
    init_target(external_sp_media_key_tap "(external)")

    set(sp_media_key_tap_loc ${third_party_loc}/SPMediaKeyTap)

    target_sources(external_sp_media_key_tap
    PRIVATE
        ${sp_media_key_tap_loc}/SPMediaKeyTap.m
        ${sp_media_key_tap_loc}/SPMediaKeyTap.h
        ${sp_media_key_tap_loc}/SPInvocationGrabbing/NSObject+SPInvocationGrabbing.m
        ${sp_media_key_tap_loc}/SPInvocationGrabbing/NSObject+SPInvocationGrabbing.h
    )
    
    target_include_directories(external_sp_media_key_tap SYSTEM
    INTERFACE
        ${sp_media_key_tap_loc}
    )
endif()
