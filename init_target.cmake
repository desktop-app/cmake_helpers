# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

set(MAXIMUM_CXX_STANDARD cxx_std_20)

function(init_target_folder target_name folder_name)
    if (NOT "${folder_name}" STREQUAL "")
        set_target_properties(${target_name} PROPERTIES FOLDER ${folder_name})
    endif()
endfunction()

function(init_target target_name) # init_target(my_target [cxx_std_..] folder_name)
    set(standard ${MAXIMUM_CXX_STANDARD})
    foreach (entry ${ARGN})
        if (${entry} STREQUAL cxx_std_14 OR ${entry} STREQUAL cxx_std_11 OR ${entry} STREQUAL cxx_std_17)
            set(standard ${entry})
        else()
            init_target_folder(${target_name} ${entry})
        endif()
    endforeach()
    target_compile_features(${target_name} PRIVATE ${standard})
    target_link_libraries(${target_name} PRIVATE desktop-app::common_options)
    set_target_properties(${target_name} PROPERTIES
        XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_WEAK YES
    )
    if (DESKTOP_APP_USE_PACKAGED)
        get_target_property(target_type ${target_name} TYPE)
        if (QT_FOUND AND target_type STREQUAL "EXECUTABLE")
            if (QT_VERSION VERSION_GREATER_EQUAL 6.2)
                cmake_language(EVAL CODE "cmake_language(DEFER CALL qt_finalize_target ${target_name})")
            endif()
            if (LINUX)
                qt_import_plugins(${target_name}
                INCLUDE
                    Qt::QGtk3ThemePlugin
                    Qt::QComposePlatformInputContextPlugin
                    Qt::QIbusPlatformInputContextPlugin
                    Qt::QXdgDesktopPortalThemePlugin
                )
            endif()
        endif()
    endif()
    if (DESKTOP_APP_SPECIAL_TARGET)
        if (MSVC)
            set_property(TARGET ${target_name} APPEND_STRING PROPERTY STATIC_LIBRARY_OPTIONS "$<$<NOT:$<CONFIG:Debug>>:/LTCG>")
        elseif (CMAKE_GENERATOR STREQUAL Xcode)
            set_target_properties(${target_name} PROPERTIES
                XCODE_ATTRIBUTE_LLVM_LTO $<IF:$<CONFIG:Debug>,NO,YES>
            )
        else()
            set_target_properties(${target_name} PROPERTIES
                INTERPROCEDURAL_OPTIMIZATION_RELEASE True
                INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO True
                INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL True
            )
        endif()
    endif()
endfunction()

# This code is not supposed to run on build machine, only on target machine.
function(init_non_host_target target_name)
    init_target(${ARGV})
    if (DESKTOP_APP_MAC_ARCH)
        set_target_properties(${target_name} PROPERTIES
            OSX_ARCHITECTURES "${DESKTOP_APP_MAC_ARCH}"
        )
    endif()
endfunction()
