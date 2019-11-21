# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_openal INTERFACE IMPORTED GLOBAL)

target_include_directories(external_openal SYSTEM
INTERFACE
    ${libs_loc}/openal-soft/include
)

target_link_directories(external_openal
INTERFACE
    ${libs_loc}/openal-soft/build/$<IF:$<CONFIG:Debug>,Debug,Release>
)

target_link_libraries(external_openal
INTERFACE
    OpenAL32
)

target_compile_definitions(external_openal
INTERFACE
    AL_LIBTYPE_STATIC
    AL_ALEXT_PROTOTYPES
)
