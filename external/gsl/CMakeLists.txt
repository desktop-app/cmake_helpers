# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_gsl INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_gsl ALIAS external_gsl)

if (DESKTOP_APP_USE_PACKAGED_GSL)
    find_path(GSL_INCLUDE_DIRS gsl/gsl)

    if (NOT GSL_INCLUDE_DIRS)
        message(FATAL_ERROR "Guidelines Support Library not found")
    endif()
else()
    target_include_directories(external_gsl SYSTEM
    INTERFACE
        ${third_party_loc}/GSL/include
    )
endif()
