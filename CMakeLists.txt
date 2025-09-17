cmake_minimum_required(VERSION 3.21)

project(mephi_reactor LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # to generate compile_commands.json

set(CMAKE_BUILD_PARALLEL_LEVEL 1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(source/common)
add_subdirectory(source/vector)
add_subdirectory(source/figures)
add_subdirectory(source/reactor)
add_subdirectory(source/mephi)
add_subdirectory(source/molecule)

find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)

add_executable(${CMAKE_PROJECT_NAME}
    source/main.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME} 
    PRIVATE
        sfml-graphics

        common
        vector
        figures
        reactor 
        mephi
        molecule
)