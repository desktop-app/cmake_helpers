# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_cmark_gfm INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_cmark_gfm ALIAS external_cmark_gfm)

if (DESKTOP_APP_USE_PACKAGED)
    block()
        set(CMAKE_FIND_LIBRARY_PREFIXES "")
        set(CMAKE_FIND_LIBRARY_SUFFIXES .cmake)
        find_library(DESKTOP_APP_CMARK_GFM cmake/cmark-gfm.cmake)
        find_library(DESKTOP_APP_CMARK_GFM_EXTENSIONS cmake-gfm-extensions/cmark-gfm-extensions.cmake)
    endblock()
    include(${DESKTOP_APP_CMARK_GFM} OPTIONAL)
    include(${DESKTOP_APP_CMARK_GFM_EXTENSIONS} OPTIONAL)
    if (TARGET libcmark-gfm AND TARGET libcmark-gfm-extensions)
        target_link_libraries(external_cmark_gfm
        INTERFACE
            libcmark-gfm-extensions
            libcmark-gfm
        )
        return()
    endif()
endif()

# Everything that follows is scoped to this block(). All variables, the
# install() shadow, and the imported policy floor are local -- siblings
# and parent never see them, so the global tdesktop config is unaffected.
block()
    set(CMAKE_POLICY_VERSION_MINIMUM 3.13)
    set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

    # Force CMP0091 NEW so CMAKE_MSVC_RUNTIME_LIBRARY (set globally by
    # tdesktop's variables.cmake) actually applies to cmark-gfm's targets.
    # Without it cmark-gfm builds against the dynamic CRT and produces
    # __imp__ thunks that fail to link against tdesktop's static-CRT code.
    set(CMAKE_POLICY_DEFAULT_CMP0091 NEW)

    # Normal-variable overrides for cmark-gfm's options(). With CMP0077
    # NEW (lifted above) these beat the hardcoded option() defaults
    # without any CACHE writes. BUILD_TESTING is intentionally NOT set:
    # CMARK_TESTS=OFF gates cmark-gfm's enable_testing() / test/ subdir,
    # so BUILD_TESTING is never consulted for this subproject and we
    # avoid a footgun for any future include(CTest) in tdesktop.
    set(CMARK_TESTS OFF)
    set(CMARK_SHARED OFF)
    set(CMARK_STATIC ON)
    set(CMARK_LIB_FUZZER OFF)

    # cmark-gfm registers unconditional install(TARGETS ...) /
    # install(EXPORT) rules, while tdesktop only embeds its targets.
    set(CMAKE_SKIP_INSTALL_RULES TRUE)

    # tdesktop configures with --warn-uninitialized -Werror=dev, which
    # turns cmark-gfm's perfectly-fine reads of optional variables
    # (LIB_SUFFIX, CMAKE_REQUIRED_*, CMAKE_LINKER_FLAGS_*, ...) into
    # hard configure errors. Suppress dev errors and warnings only
    # within this block so the rest of tdesktop keeps its strict
    # configure.
    set(CMAKE_SUPPRESS_DEVELOPER_ERRORS TRUE)
    set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS TRUE)
    if (DEFINED CACHE{CMAKE_ERROR_DEPRECATED})
        set(_cmark_gfm_error_deprecated "${CMAKE_ERROR_DEPRECATED}")
        set(_cmark_gfm_has_error_deprecated TRUE)
    else()
        set(_cmark_gfm_has_error_deprecated FALSE)
    endif()
    if (DEFINED CACHE{CMAKE_WARN_DEPRECATED})
        set(_cmark_gfm_warn_deprecated "${CMAKE_WARN_DEPRECATED}")
        set(_cmark_gfm_has_warn_deprecated TRUE)
    else()
        set(_cmark_gfm_has_warn_deprecated FALSE)
    endif()
    set(CMAKE_ERROR_DEPRECATED FALSE CACHE BOOL "" FORCE)
    set(CMAKE_WARN_DEPRECATED FALSE CACHE BOOL "" FORCE)

    # Defensive initialisation for the specific variables cmark-gfm
    # reads without checking. Empty strings are benign for cmark-gfm's
    # logic. Kept alongside the suppress flags for documentation -- the
    # suppress flags are the catch-all, these are the known offenders.
    set(CMAKE_REQUIRED_DEFINITIONS "")  # CheckFileOffsetBits.cmake:23,28
    set(CMAKE_REQUIRED_FLAGS "")
    set(CMAKE_REQUIRED_INCLUDES "")
    set(CMAKE_REQUIRED_LIBRARIES "")
    set(CMAKE_REQUIRED_LINK_OPTIONS "")
    set(CMAKE_LINKER_FLAGS_DEBUG "")    # src/CMakeLists.txt:92
    set(CMAKE_LINKER_FLAGS_RELEASE "")  # src/CMakeLists.txt:95
    set(LIB_SUFFIX "")                  # src/CMakeLists.txt:155 + extensions/CMakeLists.txt:69,82

    add_subdirectory(${third_party_loc}/cmark-gfm cmark-gfm-build EXCLUDE_FROM_ALL)
    if (_cmark_gfm_has_error_deprecated)
        set(CMAKE_ERROR_DEPRECATED "${_cmark_gfm_error_deprecated}" CACHE BOOL "" FORCE)
    else()
        unset(CMAKE_ERROR_DEPRECATED CACHE)
    endif()
    if (_cmark_gfm_has_warn_deprecated)
        set(CMAKE_WARN_DEPRECATED "${_cmark_gfm_warn_deprecated}" CACHE BOOL "" FORCE)
    else()
        unset(CMAKE_WARN_DEPRECATED CACHE)
    endif()
endblock()

target_link_libraries(external_cmark_gfm
INTERFACE
    libcmark-gfm-extensions_static
    libcmark-gfm_static
)

# Group cmark-gfm's targets under (external) in the IDE's solution view.
# init_target -- which other tdesktop ThirdParty wrappers use to get the
# folder set -- isn't applied here because the targets come from
# add_subdirectory(), so set FOLDER directly.
set_target_properties(libcmark-gfm_static libcmark-gfm-extensions_static
    PROPERTIES FOLDER "(external)")

# cmark-gfm's static targets carry CMARK_GFM_STATIC_DEFINE /
# CMARK_GFM_EXTENSIONS_STATIC_DEFINE via generate_export_header, but only
# on the targets themselves. Restate them on our INTERFACE wrapper so
# consumers of desktop-app::external_cmark_gfm don't need to know.
target_compile_definitions(external_cmark_gfm
INTERFACE
    CMARK_GFM_STATIC_DEFINE
    CMARK_GFM_EXTENSIONS_STATIC_DEFINE
)

# Restate include paths so consumers find both the source headers and the
# generated cmark-gfm_export.h / cmark-gfm_version.h / config.h that land
# in the build dir.
target_include_directories(external_cmark_gfm SYSTEM
INTERFACE
    ${third_party_loc}/cmark-gfm/src
    ${third_party_loc}/cmark-gfm/extensions
    ${CMAKE_CURRENT_BINARY_DIR}/cmark-gfm-build/src
    ${CMAKE_CURRENT_BINARY_DIR}/cmark-gfm-build/extensions
)
