# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_jemalloc INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_jemalloc ALIAS external_jemalloc)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(JEMALLOC IMPORTED_TARGET jemalloc)

    if (JEMALLOC_FOUND)
        target_link_libraries(external_jemalloc INTERFACE PkgConfig::JEMALLOC)
    endif()
endif()

if (NOT JEMALLOC_FOUND)
    add_library(external_jemalloc_bundled STATIC IMPORTED GLOBAL)

    include(ExternalProject)
    ExternalProject_Add(jemalloc
        URL https://github.com/jemalloc/jemalloc/releases/download/5.2.1/jemalloc-5.2.1.tar.bz2
        CONFIGURE_COMMAND cd "${CMAKE_CURRENT_BINARY_DIR}/jemalloc-prefix/src/jemalloc" && ./configure
        BUILD_IN_SOURCE 1
    )

    ExternalProject_Get_property(jemalloc SOURCE_DIR)
    ExternalProject_Get_property(jemalloc BINARY_DIR)

    file(MAKE_DIRECTORY "${SOURCE_DIR}/include")

    set_target_properties(external_jemalloc_bundled PROPERTIES
        IMPORTED_LOCATION "${BINARY_DIR}/lib/libjemalloc.a"
    )

    target_include_directories(external_jemalloc_bundled
    INTERFACE
        ${SOURCE_DIR}/include
    )

    target_link_libraries(external_jemalloc_bundled
    INTERFACE
        dl
        pthread
    )

    add_dependencies(external_jemalloc_bundled jemalloc)

    target_link_libraries(external_jemalloc
    INTERFACE
        -Wl,--whole-archive
        $<TARGET_FILE:external_jemalloc_bundled>
        -Wl,--no-whole-archive
        external_jemalloc_bundled
    )
endif()
