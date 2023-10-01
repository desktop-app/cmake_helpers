# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(common_options INTERFACE)
add_library(desktop-app::common_options ALIAS common_options)

target_compile_definitions(common_options
INTERFACE
    $<$<CONFIG:Debug>:_DEBUG>
    QT_NO_KEYWORDS
    QT_NO_CAST_FROM_BYTEARRAY
    QT_IMPLICIT_QCHAR_CONSTRUCTION
    QT_DEPRECATED_WARNINGS_SINCE=0x051500
)

if (DESKTOP_APP_DISABLE_X11_INTEGRATION)
    target_compile_definitions(common_options
    INTERFACE
        DESKTOP_APP_DISABLE_X11_INTEGRATION
    )
endif()

function(check_alias_template_deduction) # isolate scope
    include(CheckCXXSourceCompiles)
    set(CMAKE_REQUIRED_FLAGS -std=c++20)
    check_cxx_source_compiles("
    template<typename A, typename B>
    struct Foo {
        Foo(A const &a, B const &b)
        : a_(a), b_(b) {
        }

        A a_;
        B b_;
    };

    template<typename A, typename B>
    using Bar = Foo<A, B>;

    int main() {
        Bar s{1, 2};
        return 0;
    }
    " DESKTOP_APP_ALIAS_TEMPLATE_DEDUCTION_SUPPORTED)
    if (NOT DESKTOP_APP_ALIAS_TEMPLATE_DEDUCTION_SUPPORTED)
        message(FATAL_ERROR "Class template argument deduction for alias templates is not supported by the compiler")
    endif()
endfunction()
check_alias_template_deduction()

if (WIN32)
    include(cmake/options_win.cmake)
elseif (APPLE)
    include(cmake/options_mac.cmake)
elseif (LINUX)
    include(cmake/options_linux.cmake)
else()
    message(FATAL_ERROR "Unknown platform type")
endif()
