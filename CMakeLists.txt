cmake_minimum_required(VERSION 3.28)

project(memory-game)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        include/Game/
        include/Game/Units
        include/Game/Units/SpecialUnit
        include/UI/
)

add_subdirectory(source)