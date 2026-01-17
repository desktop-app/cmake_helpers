# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

if (DESKTOP_APP_USE_PACKAGED_SKOTTIE AND NOT DESKTOP_APP_USE_PACKAGED_LAZY)
    add_library(external_skottie INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_skottie ALIAS external_skottie)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(RLOTTIE REQUIRED IMPORTED_TARGET skottie)
    target_link_libraries(external_skottie INTERFACE PkgConfig::SKOTTIE)
else()
    add_library(external_skottie INTERFACE IMPORTED GLOBAL)
    add_library(desktop-app::external_skottie ALIAS external_skottie)
    
    target_compile_definitions(external_skottie
    INTERFACE
        SK_SUPPORT_GPU=0
    )
    target_include_directories(external_skottie SYSTEM
    INTERFACE
        ${libs_loc}/skia
    )

    set(lib_part $<IF:$<CONFIG:Debug>,Debug,Release>/lib_)
    set(lib_prefix ${libs_loc}/skia/out)

    target_link_libraries(external_openal
    INTERFACE
        ${lib_prefix}/src/codec/${lib_part}codec.lib
        ${lib_prefix}/src/images/${lib_part}images.lib
        ${lib_prefix}/src/image/${lib_part}image.lib
        ${lib_prefix}/src/opts/${lib_part}opts.lib
        ${lib_prefix}/src/shaders/${lib_part}shaders.lib
        ${lib_prefix}/third_party/skcms/${lib_part}skcms.lib
        ${lib_prefix}/src/sksl/${lib_part}sksl.lib
        ${lib_prefix}/src/utils/${lib_part}utils.lib
        ${lib_prefix}/src/core/${lib_part}core.lib
        ${lib_prefix}/src/effects/${lib_part}effects.lib
        ${lib_prefix}/src/fonts/${lib_part}fonts.lib
        ${lib_prefix}/src/pathops/${lib_part}pathops.lib
        ${lib_prefix}/src/sfnt/${lib_part}sfnt.lib
        ${lib_prefix}/src/ports/${lib_part}ports.lib
        ${lib_prefix}/modules/sksg/${lib_part}sksg.lib
        ${lib_prefix}/modules/skshaper/${lib_part}skshaper.lib
        ${lib_prefix}/lib/${lib_part}skottie.lib
    )
endif()
