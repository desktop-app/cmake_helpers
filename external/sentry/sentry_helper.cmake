# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(sentry_helper STATIC)
init_target(sentry_helper "(external)")
add_library(desktop-app::sentry_helper ALIAS sentry_helper)

include(validate_crashpad_handler.cmake)

nice_target_sources(sentry_helper ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
    sentry_helper.cpp
)

validate_crashpad_handler(sentry_helper)

target_link_libraries(sentry_helper
PUBLIC
    desktop-app::external_openssl
)

if (NOT DESKTOP_APP_SPECIAL_TARGET STREQUAL "")
    target_compile_definitions(sentry_helper
    PRIVATE
        SENTRY_HELPER_SPECIAL_TARGET=${DESKTOP_APP_SPECIAL_TARGET}
    )
endif()
