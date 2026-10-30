# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_mtproto_engine INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_mtproto_engine ALIAS external_mtproto_engine)

# Only the Windows recipe installs the header so far, and an imported target
# that names a missing include directory fails the configuration.
if (WIN32 AND NOT DESKTOP_APP_USE_PACKAGED)
    target_include_directories(external_mtproto_engine SYSTEM
    INTERFACE
        ${libs_loc}/tdesktop_rust/out/include/mtproto_engine
    )
endif()

target_link_libraries(external_mtproto_engine
INTERFACE
    $<TARGET_NAME_IF_EXISTS:desktop-app::external_tdesktop_rust>
)
