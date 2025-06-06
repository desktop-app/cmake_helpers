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
    find_package(PkgConfig)
    if (PkgConfig_FOUND)
        pkg_check_modules(DESKTOP_APP_JEMALLOC jemalloc)
    endif()

    if (DESKTOP_APP_JEMALLOC_FOUND)
        target_include_directories(external_jemalloc SYSTEM
        INTERFACE
            ${DESKTOP_APP_JEMALLOC_INCLUDE_DIRS}
        )

        target_link_libraries(external_jemalloc
        INTERFACE
            -Wl,--push-state,--no-as-needed,--whole-archive,${DESKTOP_APP_JEMALLOC_LINK_LIBRARIES},--pop-state
        )

        return()
    endif()
endif()

include(ProcessorCount)
ProcessorCount(N)

include(ExternalProject)
ExternalProject_Add(jemalloc
    URL ${third_party_loc}/jemalloc
    CONFIGURE_COMMAND env
        "EXTRA_CFLAGS=-fno-lto -fno-use-linker-plugin"
        "EXTRA_CXXFLAGS=-fno-lto -fno-use-linker-plugin"
        ./autogen.sh --disable-shared
    BUILD_COMMAND make $<$<NOT:$<EQUAL:${N},0>>:-j${N}>
    BUILD_IN_SOURCE 1
    STEP_TARGETS build
    EXCLUDE_FROM_ALL TRUE
    BUILD_BYPRODUCTS <SOURCE_DIR>/lib/libjemalloc_pic.a
)

ExternalProject_Get_property(jemalloc SOURCE_DIR)
file(MAKE_DIRECTORY "${SOURCE_DIR}/include")

target_include_directories(external_jemalloc SYSTEM
INTERFACE
    ${SOURCE_DIR}/include
)

target_link_libraries(external_jemalloc
INTERFACE
    -Wl,--push-state,--whole-archive,${SOURCE_DIR}/lib/libjemalloc_pic.a,--pop-state
)

add_dependencies(external_jemalloc jemalloc-build)
