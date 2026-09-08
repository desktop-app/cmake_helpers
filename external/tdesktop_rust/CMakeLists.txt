# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

# Every Rust dependency is built into this one archive. A Rust staticlib
# carries its own copy of the standard library, so two of them define the
# runtime symbols that must stay global twice and the linker refuses the
# binary. Building them as one crate leaves a single runtime, and a single
# allocator, panic hook and backtrace machinery at run time.

if (DESKTOP_APP_USE_PACKAGED OR LINUX)
    find_library(DESKTOP_APP_TDESKTOP_RUST_LIBRARY tdesktop_rust)
    if (NOT DESKTOP_APP_TDESKTOP_RUST_LIBRARY)
        return()
    endif()
endif()

add_library(external_tdesktop_rust_native STATIC IMPORTED GLOBAL)
add_library(external_tdesktop_rust INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_tdesktop_rust ALIAS external_tdesktop_rust)

if (DESKTOP_APP_USE_PACKAGED OR LINUX)
    set_target_properties(external_tdesktop_rust_native PROPERTIES
        IMPORTED_LOCATION "${DESKTOP_APP_TDESKTOP_RUST_LIBRARY}"
    )
elseif (WIN32)
    set(tdesktop_rust_loc ${libs_loc}/tdesktop_rust/out)
    set_target_properties(external_tdesktop_rust_native PROPERTIES
        IMPORTED_LOCATION "${tdesktop_rust_loc}/lib/tdesktop_rust.lib"
        IMPORTED_LOCATION_DEBUG "${tdesktop_rust_loc}/lib/tdesktop_rust.lib"
        IMPORTED_LOCATION_RELEASE "${tdesktop_rust_loc}/lib/tdesktop_rust.lib"
    )
elseif (APPLE)
    set_target_properties(external_tdesktop_rust_native PROPERTIES
        IMPORTED_LOCATION ${libs_loc}/local/lib/libtdesktop_rust.a
    )
endif()

target_link_libraries(external_tdesktop_rust
INTERFACE
    external_tdesktop_rust_native
)

# What the Rust standard library inside the archive needs resolved. cargo
# reports the list through --print native-static-libs while the stage builds.
# On macOS it asks only for CoreFoundation, iconv and the C runtime, which the
# toolchain links anyway.
if (WIN32)
    target_link_libraries(external_tdesktop_rust
    INTERFACE
        advapi32
        bcrypt
        dbghelp
        kernel32
        ntdll
        userenv
        ws2_32
    )
endif()
