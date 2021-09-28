# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_jemalloc INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_jemalloc ALIAS external_jemalloc)

if (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    return()
endif()

if (DESKTOP_APP_USE_PACKAGED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(JEMALLOC IMPORTED_TARGET jemalloc)

    if (JEMALLOC_FOUND)
        target_link_libraries(external_jemalloc
        INTERFACE
            -Wl,--no-as-needed
            PkgConfig::JEMALLOC
            ${JEMALLOC_LINK_LIBRARIES}
            -Wl,--as-needed
        )
    endif()
endif()

if (NOT JEMALLOC_FOUND)
    add_library(external_jemalloc_bundled STATIC IMPORTED GLOBAL)
    set(jemalloc_loc ${third_party_loc}/jemalloc)

    include(ExternalProject)
    ExternalProject_Add(jemalloc
        SOURCE_DIR ${jemalloc_loc}
        CONFIGURE_COMMAND cd "${jemalloc_loc}" && export EXTRA_CFLAGS=-fstack-protector-all\ -fPIC\ -D_FORTIFY_SOURCE=2 && export EXTRA_CXXFLAGS=-fstack-protector-all\ -fPIC\ -D_FORTIFY_SOURCE=2 && CC=clang CXX=clang++ ./autogen.sh
        INSTALL_COMMAND :
        BUILD_IN_SOURCE 1
        BUILD_BYPRODUCTS ${jemalloc_loc}/lib/libjemalloc.a
    )

    file(MAKE_DIRECTORY "${jemalloc_loc}/include")

    set_target_properties(external_jemalloc_bundled PROPERTIES
        IMPORTED_LOCATION "${jemalloc_loc}/lib/libjemalloc.a"
    )

    target_include_directories(external_jemalloc_bundled SYSTEM
    INTERFACE
        ${jemalloc_loc}/include
    )

    target_link_libraries(external_jemalloc_bundled
    INTERFACE
        ${CMAKE_DL_LIBS}
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
