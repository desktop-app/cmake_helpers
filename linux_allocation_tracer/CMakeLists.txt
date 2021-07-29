# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(linux_allocation_tracer STATIC)
add_library(desktop-app::linux_allocation_tracer ALIAS linux_allocation_tracer)

nice_target_sources(linux_allocation_tracer ${CMAKE_CURRENT_SOURCE_DIR}
PRIVATE
    linux_allocation_tracer.cpp
    linux_allocation_tracer.h
)

add_executable(allocation_trace_reader WIN32)
init_target(allocation_trace_reader)

target_sources(allocation_trace_reader
PRIVATE
    linux_allocation_trace_reader.cpp
)

set_target_properties(allocation_trace_reader PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
