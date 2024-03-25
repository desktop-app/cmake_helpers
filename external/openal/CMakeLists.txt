# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_openal INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_openal ALIAS external_openal)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(OpenAL)
    if (OPENAL_FOUND)
        target_include_directories(external_openal SYSTEM INTERFACE ${OPENAL_INCLUDE_DIR})
        target_link_libraries(external_openal INTERFACE ${OPENAL_LIBRARY})
        return()
    endif()

    find_package(OpenAL REQUIRED CONFIG)
    target_link_libraries(external_openal INTERFACE OpenAL::OpenAL)
    return()
elseif (WIN32)
    target_include_directories(external_openal SYSTEM
    INTERFACE
        ${libs_loc}/openal-soft/include
        ${libs_loc}/openal-soft/include/AL
    )
    target_link_libraries(external_openal
    INTERFACE
        ${libs_loc}/openal-soft/build/$<IF:$<CONFIG:Debug>,Debug,RelWithDebInfo>/OpenAL32.lib
    )
elseif (APPLE)
    target_include_directories(external_openal SYSTEM
    INTERFACE
        ${libs_loc}/local/include
        ${libs_loc}/local/include/AL
    )
    target_link_libraries(external_openal
    INTERFACE
        ${libs_loc}/local/lib/libopenal.a
    )
else()
    target_include_directories(external_openal SYSTEM
    INTERFACE
        /usr/local/include/AL
    )
    target_link_static_libraries(external_openal
    INTERFACE
        openal
    )
endif()

target_compile_definitions(external_openal
INTERFACE
    AL_LIBTYPE_STATIC
)
