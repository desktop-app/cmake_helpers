# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

function(desktop_app_parse_version file)
    file(STRINGS ${file} lines)
    foreach (line ${lines})
        string(REPLACE " " ";" parts ${line})
        list(GET parts 0 name)
        if (${name} STREQUAL "AppVersionOriginal")
            list(LENGTH parts length)
            math(EXPR index "${length} - 1")
            list(GET parts ${index} version)
            break()
        endif()
    endforeach()

    if (NOT ${version} MATCHES "[0-9]+\.[0-9]+(\.[0-9]+(\.([0-9]+|beta))?)?")
        message(FATAL_ERROR "Bad version: ${version}, check ${file}")
    endif()

    message("Version: ${version}")

    set(beta 0)
    set(beta_bool "false")
    set(alpha 0)

    string(REPLACE "." ";" components ${version})
    list(LENGTH components components_count)
    list(GET components 0 major)
    list(GET components 1 minor)
    if (${components_count} GREATER 2)
        list(GET components 2 patch)
        if (${components_count} GREATER 3)
            list(GET components 3 alpha)
            if (${alpha} STREQUAL beta)
                set(beta 1)
                set(beta_bool "true")
                set(alpha 0)
            endif()
        endif()
    else()
        set(patch 0)
    endif()

    if (DESKTOP_APP_SPECIAL_TARGET STREQUAL "")
        set(alpha 0)
    endif()

    math(EXPR version_int "${major} * 1000000 + ${minor} * 1000 + ${patch}")

    set(str_major ${major}.${minor})
    set(str_normal ${str_major}.${patch})
    if (${patch} STREQUAL 0)
        set(str_small ${str_major})
    else()
        set(str_small ${str_normal})
    endif()

    if (NOT ${alpha} STREQUAL 0)
        math(EXPR version_int_alpha "${version_int} * 1000 + ${alpha}")
    else()
        set(version_int_alpha 0)
    endif()
    set(version_dot ${str_normal}.${alpha})
    string(REPLACE "." "," version_comma ${version_dot})

    set(desktop_app_version_int ${version_int} PARENT_SCOPE)
    set(desktop_app_version_int_alpha ${version_int_alpha} PARENT_SCOPE)
    set(desktop_app_version_string ${str_normal} PARENT_SCOPE)
    set(desktop_app_version_string_small ${str_small} PARENT_SCOPE)
    set(desktop_app_version_beta_bool ${beta_bool} PARENT_SCOPE)
    set(desktop_app_version_dot ${version_dot} PARENT_SCOPE)
    set(desktop_app_version_comma ${version_dot} PARENT_SCOPE)
    set(desktop_app_version_cmake ${version_dot} PARENT_SCOPE)
endfunction()
