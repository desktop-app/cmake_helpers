add_executable(external_parsetrigrams)
init_target(external_parsetrigrams "(external)")

set(sonnet_loc ${third_party_loc}/sonnet)

target_sources(external_parsetrigrams
PRIVATE
    ${sonnet_loc}/data/parsetrigrams.cpp
)

target_link_libraries(external_parsetrigrams
PUBLIC
    desktop-app::external_qt
)
