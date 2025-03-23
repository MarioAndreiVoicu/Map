cmake_minimum_required(VERSION 3.26)
project(Map LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(MapLib STATIC
    src/RedBlackTree.cppm
    src/Map.cppm
)

target_include_directories(MapLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(MapExample
    example/example.cpp
)

target_link_libraries(MapExample PRIVATE MapLib)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MapExample)

# Hide common CMake-generated targets from the Visual Studio solution
if(TARGET ALL_BUILD)
  set_target_properties(ALL_BUILD PROPERTIES VS_HIDE_FROM_SOLUTION TRUE)
endif()

if(TARGET ZERO_CHECK)
  set_target_properties(ZERO_CHECK PROPERTIES VS_HIDE_FROM_SOLUTION TRUE)
endif()
