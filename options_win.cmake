# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

target_compile_definitions(common_options
INTERFACE
    WIN32
    WIN32_LEAN_AND_MEAN
    _WINDOWS
    _SCL_SECURE_NO_WARNINGS
    NOMINMAX
    NOSERVICE
    NOMCX
    NOIME
    NOSOUND
    NOCOMM
    NOKANJI
    NORPC
    NOPROXYSTUB
    NOIMAGE
    NOTAPE
    UNICODE
    _UNICODE
)
if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    target_compile_options(common_options
    INTERFACE
        /permissive-
        # /Qspectre
        /utf-8
        /W4
        /MP     # Enable multi process build.
        /EHsc   # Catch C++ exceptions only, extern C functions never throw a C++ exception.
        /w15038 # wrong initialization order
        /w14265 # class has virtual functions, but destructor is not virtual
        /wd4018 # 'token' : signed/unsigned mismatch
        /wd4100 # 'identifier' : unreferenced formal parameter
        /wd4242 # 'identifier': conversion from 'type1' to 'type2', possible loss of data
        /wd4244 # 'argument' : conversion from 'type1' to 'type2', possible loss of data
        /wd4245 # 'conversion' : conversion from 'type1' to 'type2', signed/unsigned mismatch
        /wd4267 # 'var' : conversion from 'size_t' to 'type', possible loss of data
        /wd4305 # 'conversion': truncation from 'type1' to 'type2'
        /wd4324 # 'structname': structure was padded due to alignment specifier
        /wd4389 # 'equality-operator' : signed/unsigned mismatch
        /wd4456 # declaration of 'identifier' hides previous local declaration
        /wd4457 # declaration of 'identifier' hides function parameter
        /wd4458 # declaration of 'identifier' hides class member
        /wd4459 # declaration of 'identifier' hides global declaration
        /wd4611 # interaction between 'function' and C++ object destruction is non-portable
        /wd4702 # unreachable code
        /Zi

        # Taken from Qt 6.
        # https://developercommunity.visualstudio.com/content/problem/139261/msvc-incorrectly-defines-cplusplus.html
        # No support for the flag in upstream CMake as of 3.17.
        # https://gitlab.kitware.com/cmake/cmake/issues/18837
        /Zc:__cplusplus
    )

    target_link_options(common_options
    INTERFACE
        $<IF:$<CONFIG:Debug>,/NODEFAULTLIB:LIBCMT,/DEBUG;/OPT:REF>
        $<$<BOOL:${DESKTOP_APP_NO_PDB}>:/DEBUG:NONE>
        /INCREMENTAL:NO
    )

    if (DESKTOP_APP_ASAN)
        target_compile_options(common_options INTERFACE /fsanitize=address)

        # https://developercommunity.visualstudio.com/t/Linker-error-LNK2038-when-using-Parallel/10512721
        target_compile_definitions(common_options
        INTERFACE
            _DISABLE_VECTOR_ANNOTATION
            _DISABLE_STRING_ANNOTATION
        )
    endif()

    if (build_win64 OR build_winarm)
        target_compile_options(common_options
        INTERFACE
            /bigobj # scheme.cpp has too many sections.
        )
    else()
        # target_compile_options(common_options
        # INTERFACE
        #     /fp:except # Crash-report fp exceptions in 32 bit build.
        # )
        target_link_options(common_options
        INTERFACE
            /LARGEADDRESSAWARE # Allow more than 2 GB in 32 bit application.
        )
    endif()

    if (DESKTOP_APP_SPECIAL_TARGET)
        target_compile_options(common_options
        INTERFACE
            /WX
            $<IF:$<CONFIG:Debug>,,/GL>
        )
        target_link_options(common_options
        INTERFACE
            $<IF:$<CONFIG:Debug>,,/LTCG>
            $<IF:$<CONFIG:Debug>,,/LTCGOUT:>
        )
    endif()
elseif (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
    target_compile_definitions(common_options
    INTERFACE
        WINVER=0x0601
        _WIN32_WINNT=0x0601
    )

    target_compile_options(common_options
    INTERFACE
        -fpermissive
    )

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        target_compile_options(common_options
        INTERFACE
            -fms-extensions
            -femulated-tls
        )

        target_link_options(common_options
        INTERFACE
            -fuse-ld=lld
        )
    endif()
endif()

target_link_libraries(common_options
INTERFACE
    winmm
    imm32
    ws2_32
    kernel32
    user32
    gdi32
    winspool
    comdlg32
    advapi32
    shell32
    ole32
    oleaut32
    uuid
    odbc32
    odbccp32
    Shlwapi
    Iphlpapi
    Gdiplus
    Strmiids
    Netapi32
    Userenv
    Version
    Dwmapi
    UxTheme
    Wtsapi32
    Crypt32
    Propsys
    Bcrypt
)

if (build_winstore)
    target_compile_definitions(common_options INTERFACE OS_WIN_STORE)
endif()
