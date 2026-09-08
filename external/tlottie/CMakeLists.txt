# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_tlottie INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_tlottie ALIAS external_tlottie)

if (DESKTOP_APP_USE_PACKAGED OR LINUX)
    if (NOT TARGET desktop-app::external_tdesktop_rust)
        find_library(DESKTOP_APP_TLOTTIE_LIBRARY tlottie REQUIRED)
        target_link_libraries(external_tlottie INTERFACE "${DESKTOP_APP_TLOTTIE_LIBRARY}")
    endif()
    find_path(DESKTOP_APP_TLOTTIE_INCLUDE_DIR tlottie.h PATH_SUFFIXES tlottie REQUIRED)
    set(tlottie_include "${DESKTOP_APP_TLOTTIE_INCLUDE_DIR}")
elseif (WIN32)
    set(tlottie_include ${libs_loc}/tdesktop_rust/out/include/tlottie)
elseif (APPLE)
    set(tlottie_include ${libs_loc}/local/include/tlottie)
endif()

target_include_directories(external_tlottie SYSTEM
INTERFACE
    ${tlottie_include}
)

target_link_libraries(external_tlottie
INTERFACE
    $<TARGET_NAME_IF_EXISTS:desktop-app::external_tdesktop_rust>
)
