# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

target_compile_definitions(common_options
INTERFACE
    WIN32
    _WINDOWS
    _SCL_SECURE_NO_WARNINGS
    NOMINMAX
    UNICODE
    _UNICODE
)
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(common_options
    INTERFACE
        /permissive-
        # /Qspectre
        /utf-8
        /W1
        /WX
        /MP     # Enable multi process build.
        /EHsc   # Catch C++ exceptions only, extern C functions never throw a C++ exception.
        /w14834 # [[nodiscard]]
        /w15038 # wrong initialization order
        /w14265 # class has virtual functions, but destructor is not virtual
        /w14101 # 'identifier' : unreferenced local variable
        /wd4068 # Disable "warning C4068: unknown pragma"
        /wd4267 # 'initializing': conversion from 'size_t' to 'int', possible loss of data.
        /wd4244 # '=': conversion from 'size_t' to 'int', possible loss of data.
        /Zc:wchar_t- # don't tread wchar_t as builtin type
        /Zi
    )

    target_link_options(common_options
    INTERFACE
        $<IF:$<CONFIG:Debug>,/NODEFAULTLIB:LIBCMT,/DEBUG;/OPT:REF>
        $<$<BOOL:${DESKTOP_APP_NO_PDB}>:/DEBUG:NONE>
    )

    if (build_win64)
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
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
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
)

if (build_winstore)
    target_compile_definitions(common_options INTERFACE OS_WIN_STORE)
endif()
