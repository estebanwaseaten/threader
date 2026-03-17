cmake_minimum_required(VERSION 3.0.0)

project(threader)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

set(HEADER_LIST
    "include/threader.h"
    "src/debug.h"
)
set(SOURCE_LIST
    "src/threader.cpp"
)

add_library(${PROJECT_NAME} SHARED ${SOURCE_LIST} )

target_include_directories(${PROJECT_NAME}
    PUBLIC include
    PRIVATE src
)

target_link_libraries(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
