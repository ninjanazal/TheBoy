# CMakeList.txt : CMake project for TheBoy, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project (TheBoy LANGUAGES CXX)

#set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "SFML\\extlibs\\headers")   
#set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "SFML\\extlibs\\libs-msvc\\x64")

set(SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/out/lib/cmake/SFML/)
set(SFML_STATIC_LIBRARIES TRUE)

find_package(SFML 2.5 COMPONENTS graphics system window audio REQUIRED)

add_subdirectory(src)