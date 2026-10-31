# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_mtproto_engine INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_mtproto_engine ALIAS external_mtproto_engine)

# Packaged macOS and Windows recipes do not install the header yet.
if (WIN32 AND NOT DESKTOP_APP_USE_PACKAGED)
    target_include_directories(external_mtproto_engine SYSTEM
    INTERFACE
        ${libs_loc}/tdesktop_rust/out/include/mtproto_engine
    )
elseif (APPLE AND NOT DESKTOP_APP_USE_PACKAGED)
    # Not SYSTEM: clang's -MMD must track engine header changes.
    set_target_properties(external_mtproto_engine PROPERTIES SYSTEM OFF)
    target_include_directories(external_mtproto_engine
    INTERFACE
        ${libs_loc}/local/include/mtproto_engine
    )
elseif (LINUX)
    find_path(DESKTOP_APP_MTPROTO_ENGINE_INCLUDE_DIR mtproto_engine.h PATH_SUFFIXES mtproto_engine REQUIRED)
    target_include_directories(external_mtproto_engine SYSTEM
    INTERFACE
        ${DESKTOP_APP_MTPROTO_ENGINE_INCLUDE_DIR}
    )
endif()

target_link_libraries(external_mtproto_engine
INTERFACE
    $<TARGET_NAME_IF_EXISTS:desktop-app::external_tdesktop_rust>
)
