# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_rlottie OBJECT)
add_library(desktop-app::external_rlottie ALIAS external_rlottie)
init_target(external_rlottie "(external)")

get_filename_component(src_loc . REALPATH)
set(rlottie_loc ${third_party_loc}/rlottie)

target_sources(external_rlottie
PRIVATE
    ${src_loc}/config/config.h

    ${rlottie_loc}/inc/rlottie.h
    ${rlottie_loc}/inc/rlottie_capi.h
    ${rlottie_loc}/inc/rlottiecommon.h

    ${rlottie_loc}/src/lottie/lottieanimation.cpp
    ${rlottie_loc}/src/lottie/lottieitem.cpp
    ${rlottie_loc}/src/lottie/lottieitem.h
    ${rlottie_loc}/src/lottie/lottiekeypath.cpp
    ${rlottie_loc}/src/lottie/lottiekeypath.h
    ${rlottie_loc}/src/lottie/lottieloader.cpp
    ${rlottie_loc}/src/lottie/lottieloader.h
    ${rlottie_loc}/src/lottie/lottiemodel.cpp
    ${rlottie_loc}/src/lottie/lottiemodel.h
    ${rlottie_loc}/src/lottie/lottieparser.cpp
    ${rlottie_loc}/src/lottie/lottieparser.h
    ${rlottie_loc}/src/lottie/lottieproxymodel.cpp
    ${rlottie_loc}/src/lottie/lottieproxymodel.h

    ${rlottie_loc}/src/vector/freetype/v_ft_math.cpp
    ${rlottie_loc}/src/vector/freetype/v_ft_math.h
    ${rlottie_loc}/src/vector/freetype/v_ft_raster.cpp
    ${rlottie_loc}/src/vector/freetype/v_ft_raster.h
    ${rlottie_loc}/src/vector/freetype/v_ft_stroker.cpp
    ${rlottie_loc}/src/vector/freetype/v_ft_stroker.h
    ${rlottie_loc}/src/vector/freetype/v_ft_types.h

    # ${rlottie_loc}/src/vector/pixman/pixman-arm-neon-asm.h
    # ${rlottie_loc}/src/vector/pixman/pixman-arm-neon-asm.S
    ${rlottie_loc}/src/vector/pixman/vregion.cpp
    ${rlottie_loc}/src/vector/pixman/vregion.h

    ${rlottie_loc}/src/vector/vbezier.cpp
    ${rlottie_loc}/src/vector/vbezier.h
    ${rlottie_loc}/src/vector/vbitmap.cpp
    ${rlottie_loc}/src/vector/vbitmap.h
    ${rlottie_loc}/src/vector/vbrush.cpp
    ${rlottie_loc}/src/vector/vbrush.h
    ${rlottie_loc}/src/vector/vcompositionfunctions.cpp
    ${rlottie_loc}/src/vector/vcowptr.h
    ${rlottie_loc}/src/vector/vdasher.cpp
    ${rlottie_loc}/src/vector/vdasher.h
    ${rlottie_loc}/src/vector/vdebug.cpp
    ${rlottie_loc}/src/vector/vdebug.h
    ${rlottie_loc}/src/vector/vdrawable.cpp
    ${rlottie_loc}/src/vector/vdrawable.h
    ${rlottie_loc}/src/vector/vdrawhelper.cpp
    ${rlottie_loc}/src/vector/vdrawhelper.h
    ${rlottie_loc}/src/vector/vdrawhelper_neon.cpp
    ${rlottie_loc}/src/vector/vdrawhelper_sse2.cpp
    ${rlottie_loc}/src/vector/velapsedtimer.cpp
    ${rlottie_loc}/src/vector/velapsedtimer.h
    ${rlottie_loc}/src/vector/vglobal.h
    ${rlottie_loc}/src/vector/vimageloader.cpp
    ${rlottie_loc}/src/vector/vimageloader.h
    ${rlottie_loc}/src/vector/vinterpolator.cpp
    ${rlottie_loc}/src/vector/vinterpolator.h
    ${rlottie_loc}/src/vector/vline.h
    ${rlottie_loc}/src/vector/vmatrix.cpp
    ${rlottie_loc}/src/vector/vmatrix.h
    ${rlottie_loc}/src/vector/vpainter.cpp
    ${rlottie_loc}/src/vector/vpainter.h
    ${rlottie_loc}/src/vector/vpath.cpp
    ${rlottie_loc}/src/vector/vpath.h
    ${rlottie_loc}/src/vector/vpathmesure.cpp
    ${rlottie_loc}/src/vector/vpathmesure.h
    ${rlottie_loc}/src/vector/vpoint.h
    ${rlottie_loc}/src/vector/vraster.cpp
    ${rlottie_loc}/src/vector/vraster.h
    ${rlottie_loc}/src/vector/vrect.cpp
    ${rlottie_loc}/src/vector/vrect.h
    ${rlottie_loc}/src/vector/vrle.cpp
    ${rlottie_loc}/src/vector/vrle.h
    ${rlottie_loc}/src/vector/vstackallocator.h
    ${rlottie_loc}/src/vector/vtaskqueue.h
)

target_compile_definitions(external_rlottie
PUBLIC
    LOT_BUILD
PRIVATE
    _USE_MATH_DEFINES
    "RAPIDJSON_ASSERT=(void)"
)

target_include_directories(external_rlottie
PUBLIC
    ${rlottie_loc}/inc
PRIVATE
    ${src_loc}/config
    ${rlottie_loc}/src/lottie
    ${rlottie_loc}/src/vector
    ${rlottie_loc}/src/vector/pixman
    ${rlottie_loc}/src/vector/freetype
)

if (WIN32)
    target_compile_options(external_rlottie
    PRIVATE
        /w44244 # 'initializing': conversion from 'double' to 'float'
        /w44251 # needs to have dll-interface to be used by clients of class
    )
endif()
