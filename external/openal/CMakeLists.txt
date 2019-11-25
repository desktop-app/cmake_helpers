# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_openal INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_openal ALIAS external_openal)

if (WIN32)
    target_include_directories(external_openal SYSTEM
    INTERFACE
        ${libs_loc}/openal-soft/include
    )
    target_link_libraries(external_openal
    INTERFACE
        ${libs_loc}/openal-soft/build/$<IF:$<CONFIG:Debug>,Debug,Release>/OpenAL32
    )
else()
    target_include_directories(external_openal SYSTEM
    INTERFACE
        /usr/local/macos/include
    )
    target_link_libraries(external_openal
    INTERFACE
        /usr/local/macos/lib/libopenal.a
    )
endif()

target_compile_definitions(external_openal
INTERFACE
    AL_LIBTYPE_STATIC
    AL_ALEXT_PROTOTYPES
)
