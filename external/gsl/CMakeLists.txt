# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_gsl INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_gsl ALIAS external_gsl)

if (DESKTOP_APP_USE_PACKAGED)
    # Microsoft.GSL uses SameMajorVersion compatibility, so a versioned
    # request would reject GSL 5.x, which is source compatible with 4.x.
    find_package(Microsoft.GSL QUIET)
    if (Microsoft.GSL_FOUND AND Microsoft.GSL_VERSION VERSION_GREATER_EQUAL 4.1.0)
        target_link_libraries(external_gsl INTERFACE Microsoft.GSL::GSL)
        return()
    endif()
endif()

# https://gitlab.kitware.com/cmake/cmake/-/issues/25222
if (NOT EXISTS ${third_party_loc}/GSL/include)
    message(FATAL_ERROR "Guidelines Support Library is not found")
endif()

target_include_directories(external_gsl SYSTEM
INTERFACE
    ${third_party_loc}/GSL/include
)
