cmake_minimum_required(VERSION 3.21)
project(RogueLike)

set(CMAKE_CXX_STANDARD 23)

add_executable(RogueLike main.cpp GameWorld.cpp MapTile.cpp Randomer.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML-2.5.1/lib/cmake/SFML)

find_package(SFML COMPONENTS system window graphics audio REQUIRED)

include_directories(C:/SFML-2.5.1/include)

target_link_libraries(RogueLike sfml-system sfml-window sfml-graphics sfml-audio)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})