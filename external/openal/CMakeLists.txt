# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_openal INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_openal ALIAS external_openal)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(OpenAL REQUIRED)
    target_include_directories(external_openal INTERFACE ${OPENAL_INCLUDE_DIR})
    target_link_libraries(external_openal INTERFACE ${OPENAL_LIBRARY})
elseif (WIN32)
    target_include_directories(external_openal SYSTEM
    INTERFACE
        ${libs_loc}/openal-soft/include
    )
    target_link_libraries(external_openal
    INTERFACE
        ${libs_loc}/openal-soft/build/$<IF:$<CONFIG:Debug>,Debug,Release>/OpenAL32.lib
    )
elseif(APPLE AND NOT build_osx)
    target_include_directories(external_openal SYSTEM
    INTERFACE
        /usr/local/macos/include
    )
    target_link_libraries(external_openal
    INTERFACE
        /usr/local/macos/lib/libopenal.a
    )
else()
    target_include_directories(external_openal SYSTEM
    INTERFACE
        /usr/local/include
    )
    target_link_static_libraries(external_openal
    INTERFACE
        openal
    )
endif()

if (NOT DESKTOP_APP_USE_PACKAGED)
    target_compile_definitions(external_openal
    INTERFACE
        AL_LIBTYPE_STATIC
    )
endif()

target_compile_definitions(external_openal
INTERFACE
    AL_ALEXT_PROTOTYPES
)
