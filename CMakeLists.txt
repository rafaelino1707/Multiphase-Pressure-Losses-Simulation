cmake_minimum_required(VERSION 3.15)

project(MultiphasePressureLossesSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(COOLPROP_STATIC_LIBRARY ON)
add_subdirectory(externals/CoolProp)

add_executable(main
    source/main.cpp
    source/pipe_geometry.cpp
    source/pipe_solver.cpp
    source/byphasic_model.cpp
)

target_include_directories(main PRIVATE
    headers
)

target_link_libraries(main PRIVATE CoolProp)