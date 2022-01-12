# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_rlottie INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_rlottie ALIAS external_rlottie)

if (DESKTOP_APP_USE_PACKAGED_RLOTTIE)
    find_package(rlottie QUIET)
    if (rlottie_FOUND)
        target_link_libraries(external_rlottie INTERFACE rlottie::rlottie)
        return()
    endif()

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(RLOTTIE REQUIRED IMPORTED_TARGET rlottie)
    target_link_libraries(external_rlottie INTERFACE PkgConfig::RLOTTIE)
    return()
endif()

add_library(external_rlottie_bundled STATIC)
init_target(external_rlottie_bundled "(external)")

get_filename_component(src_loc . REALPATH)
set(rlottie_loc ${third_party_loc}/rlottie)

target_sources(external_rlottie_bundled PRIVATE ${src_loc}/config/config.h)
nice_target_sources(external_rlottie_bundled ${rlottie_loc}
PRIVATE
    inc/rlottie.h
    inc/rlottie_capi.h
    inc/rlottiecommon.h

    src/lottie/lottieanimation.cpp
    src/lottie/lottieitem.cpp
    src/lottie/lottieitem.h
    src/lottie/lottiekeypath.cpp
    src/lottie/lottiekeypath.h
    src/lottie/lottieloader.cpp
    src/lottie/lottieloader.h
    src/lottie/lottiemodel.cpp
    src/lottie/lottiemodel.h
    src/lottie/lottieparser.cpp
    src/lottie/lottieparser.h
    src/lottie/lottieproxymodel.cpp
    src/lottie/lottieproxymodel.h

    src/vector/freetype/v_ft_math.cpp
    src/vector/freetype/v_ft_math.h
    src/vector/freetype/v_ft_raster.cpp
    src/vector/freetype/v_ft_raster.h
    src/vector/freetype/v_ft_stroker.cpp
    src/vector/freetype/v_ft_stroker.h
    src/vector/freetype/v_ft_types.h

    # src/vector/pixman/pixman-arm-neon-asm.h
    # src/vector/pixman/pixman-arm-neon-asm.S
    src/vector/pixman/vregion.cpp
    src/vector/pixman/vregion.h

    src/vector/vbezier.cpp
    src/vector/vbezier.h
    src/vector/vbitmap.cpp
    src/vector/vbitmap.h
    src/vector/vbrush.cpp
    src/vector/vbrush.h
    src/vector/vcompositionfunctions.cpp
    src/vector/vcowptr.h
    src/vector/vdasher.cpp
    src/vector/vdasher.h
    src/vector/vdebug.cpp
    src/vector/vdebug.h
    src/vector/vdrawable.cpp
    src/vector/vdrawable.h
    src/vector/vdrawhelper.cpp
    src/vector/vdrawhelper.h
    src/vector/vdrawhelper_neon.cpp
    src/vector/vdrawhelper_sse2.cpp
    src/vector/velapsedtimer.cpp
    src/vector/velapsedtimer.h
    src/vector/vglobal.h
    src/vector/vimageloader.cpp
    src/vector/vimageloader.h
    src/vector/vinterpolator.cpp
    src/vector/vinterpolator.h
    src/vector/vline.h
    src/vector/vmatrix.cpp
    src/vector/vmatrix.h
    src/vector/vpainter.cpp
    src/vector/vpainter.h
    src/vector/vpath.cpp
    src/vector/vpath.h
    src/vector/vpathmesure.cpp
    src/vector/vpathmesure.h
    src/vector/vpoint.h
    src/vector/vraster.cpp
    src/vector/vraster.h
    src/vector/vrect.cpp
    src/vector/vrect.h
    src/vector/vrle.cpp
    src/vector/vrle.h
    src/vector/vstackallocator.h
    src/vector/vtaskqueue.h
)

target_compile_definitions(external_rlottie_bundled
PUBLIC
    LOT_BUILD
PRIVATE
    _USE_MATH_DEFINES
    "RAPIDJSON_ASSERT=(void)"
    LOTTIE_DISABLE_ARM_NEON
)

target_include_directories(external_rlottie_bundled
PUBLIC
    ${rlottie_loc}/inc
PRIVATE
    ${src_loc}/config
    ${rlottie_loc}/src/lottie
    ${rlottie_loc}/src/vector
    ${rlottie_loc}/src/vector/pixman
    ${rlottie_loc}/src/vector/freetype
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(external_rlottie_bundled
    PRIVATE
        /w44244 # 'initializing': conversion from 'double' to 'float'
        /w44251 # needs to have dll-interface to be used by clients of class
    )
endif()

target_link_libraries(external_rlottie
INTERFACE
    external_rlottie_bundled
)
