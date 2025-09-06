add_library(query
    query_parser.cpp
)

target_include_directories(query
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)