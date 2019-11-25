# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

function(target_link_frameworks target_name)
    set(list ${ARGV})
    list(REMOVE_AT list 0)

    set(writing_now "")
    set(private_frameworks "")
    set(public_frameworks "")
    set(interface_frameworks "")
    foreach (entry ${list})    
        if (${entry} STREQUAL "PRIVATE" OR ${entry} STREQUAL "PUBLIC" OR ${entry} STREQUAL "INTERFACE")
            set(writing_now ${entry})
        else()
            set(full_argument "-framework ${entry}")
            if ("${writing_now}" STREQUAL "PRIVATE")
                list(APPEND private_frameworks ${full_argument})
            elseif ("${writing_now}" STREQUAL "PUBLIC")
                list(APPEND public_frameworks ${full_argument})
            elseif ("${writing_now}" STREQUAL "INTERFACE")
                list(APPEND interface_frameworks ${full_argument})
            else()
                message(FATAL_ERROR "Unknown frameworks scope for target ${target_name}")
            endif()
        endif()
    endforeach()

    if (NOT "${public_frameworks}" STREQUAL "")
        target_link_libraries(${target_name} PUBLIC ${public_frameworks})
    endif()
    if (NOT "${private_frameworks}" STREQUAL "")
        target_link_libraries(${target_name} PRIVATE ${private_frameworks})
    endif()
    if (NOT "${interface_frameworks}" STREQUAL "")
        target_link_libraries(${target_name} INTERFACE ${interface_frameworks})
    endif()
endfunction()
