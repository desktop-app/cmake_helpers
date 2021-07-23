# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_iconv INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_iconv ALIAS external_iconv)

if (APPLE AND DESKTOP_APP_USE_PACKAGED)
    find_package(Iconv REQUIRED)
    target_link_libraries(external_iconv INTERFACE Iconv::Iconv)
elseif (APPLE)
    target_link_libraries(external_iconv
    INTERFACE
        ${libs_loc}/local/lib/libiconv.a
    )
endif()
