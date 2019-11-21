# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

function(init_target_folder target_name folder_name)
    if (NOT "${folder_name}" STREQUAL "")
        set_target_properties(${target_name} PROPERTIES FOLDER ${folder_name})
    endif()
endfunction()

function(init_target target_name) # init_target(my_target folder_name)
    init_target_folder(${target_name} "${ARGV1}")
    if (WIN32)
        set_target_properties(${target_name} PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
    target_link_libraries(${target_name} PUBLIC desktop-app::common_options)
    set_target_properties(${target_name} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE True
        INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO True
        INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL True
    )
endfunction()

