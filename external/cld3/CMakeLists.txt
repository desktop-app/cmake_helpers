# This file is part of Desktop App Toolkit,
# a set of libraries for developing nice desktop applications.
#
# For license and copyright information please follow this link:
# https://github.com/desktop-app/legal/blob/master/LEGAL

add_library(external_cld3 INTERFACE IMPORTED GLOBAL)
add_library(desktop-app::external_cld3 ALIAS external_cld3)

if (DESKTOP_APP_USE_PACKAGED)
    find_package(protobuf CONFIG)
    find_library(DESKTOP_APP_CLD3_LIBRARIES NAMES cld3)
    find_path(DESKTOP_APP_CLD3_INCLUDE_DIRS NAMES nnet_language_identifier.h PATH_SUFFIXES cld3)
    if (protobuf_FOUND AND DESKTOP_APP_CLD3_LIBRARIES AND DESKTOP_APP_CLD3_INCLUDE_DIRS)
        list(APPEND DESKTOP_APP_CLD3_LIBRARIES protobuf::libprotobuf-lite)
        target_include_directories(external_cld3 SYSTEM INTERFACE ${DESKTOP_APP_CLD3_INCLUDE_DIRS})
        target_link_libraries(external_cld3 INTERFACE ${DESKTOP_APP_CLD3_LIBRARIES})
        return()
    endif()
endif()

add_library(external_cld3_bundled STATIC)
init_target(external_cld3_bundled "(external)")

set(cld3_loc ${third_party_loc}/cld3)
set(cld3_src ${cld3_loc}/src)

nice_target_sources(external_cld3_bundled ${cld3_src}
PRIVATE
    base.cc
    embedding_feature_extractor.cc
    embedding_network.cc
    feature_extractor.cc
    feature_extractor.h
    feature_types.cc
    fml_parser.cc
    language_identifier_features.cc
    lang_id_nn_params.cc
    nnet_language_identifier.cc
    registry.cc
    relevant_script_feature.cc
    sentence_features.cc
    task_context.cc
    task_context_params.cc
    unicodetext.cc
    utils.cc
    workspace.cc

    script_span/generated_entities.cc
    script_span/getonescriptspan.cc
    script_span/getonescriptspan.h
    script_span/utf8statetable.cc
    script_span/offsetmap.cc
    script_span/text_processing.cc
    script_span/text_processing.h
    script_span/fixunicodevalue.cc
)

# cld3 keeps its settings in protobuf messages that it never serializes,
# these headers replace the generated code with the fields that are used.
nice_target_sources(external_cld3_bundled ${CMAKE_CURRENT_SOURCE_DIR}/headers
PRIVATE
    cld_3/protos/feature_extractor.pb.h
    cld_3/protos/sentence.pb.h
    cld_3/protos/task_spec.pb.h
)

target_compile_definitions(external_cld3_bundled
PUBLIC
    $<$<NOT:$<CONFIG:Debug>>:NDEBUG>
)

if (NOT MSVC)
    target_compile_options_if_exists(external_cld3_bundled
    PRIVATE
        -Wno-implicit-fallthrough
    )
endif()

target_include_directories(external_cld3_bundled
PUBLIC
    ${cld3_src}
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

target_link_libraries(external_cld3
INTERFACE
    external_cld3_bundled
)
