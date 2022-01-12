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
    pkg_check_modules(JEMALLOC jemalloc)

    if (JEMALLOC_FOUND)
        target_include_directories(external_jemalloc SYSTEM
        INTERFACE
            ${JEMALLOC_INCLUDE_DIRS}
        )

        target_link_options(external_jemalloc
        INTERFACE
            -Wl,--push-state,--no-as-needed,${JEMALLOC_LINK_LIBRARIES},--pop-state
        )

        return()
    endif()
endif()

include(ExternalProject)
ExternalProject_Add(jemalloc
    URL ${third_party_loc}/jemalloc
    CONFIGURE_COMMAND cd "<SOURCE_DIR>" && export EXTRA_CFLAGS=-fstack-protector-all\ -fPIC\ -D_FORTIFY_SOURCE=2 && export EXTRA_CXXFLAGS=-fstack-protector-all\ -fPIC\ -D_FORTIFY_SOURCE=2 && CC=clang CXX=clang++ ./autogen.sh
    BUILD_IN_SOURCE 1
    STEP_TARGETS build
    EXCLUDE_FROM_ALL TRUE
    BUILD_BYPRODUCTS <SOURCE_DIR>/lib/libjemalloc.a
)

ExternalProject_Get_property(jemalloc SOURCE_DIR)
file(MAKE_DIRECTORY "${SOURCE_DIR}/include")

target_include_directories(external_jemalloc SYSTEM
INTERFACE
    ${SOURCE_DIR}/include
)

target_link_options(external_jemalloc
INTERFACE
    -Wl,--push-state,--whole-archive,${SOURCE_DIR}/lib/libjemalloc.a,--pop-state
)

add_dependencies(external_jemalloc jemalloc-build)
