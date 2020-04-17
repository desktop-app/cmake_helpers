# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_gsl INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_gsl ALIAS external_gsl)

if (DESKTOP_APP_USE_PACKAGED_GSL)
    find_package(Microsoft.GSL CONFIG REQUIRED)
    target_link_libraries(external_gsl INTERFACE Microsoft.GSL::GSL)
else()
    target_include_directories(external_gsl SYSTEM
    INTERFACE
        ${third_party_loc}/GSL/include
    )
endif()
