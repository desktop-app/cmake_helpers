# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_variant INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_variant ALIAS external_variant)

if (DESKTOP_APP_USE_PACKAGED)
    find_path(VARIANT_INCLUDE_DIRS mapbox/variant.hpp)
endif()

if (NOT VARIANT_INCLUDE_DIRS)
    target_include_directories(external_variant SYSTEM
    INTERFACE
        ${third_party_loc}/variant/include
    )
endif()
