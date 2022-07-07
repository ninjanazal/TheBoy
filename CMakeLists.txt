# CMakeList.txt : CMake project for TheBoy, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "My multi config types" FORCE)

set (CMAKE_CXX_STANDARD 17)
project (TheBoy LANGUAGES CXX)

message("[ :: USING ${TARGETCONFIG} :: ]")
add_subdirectory(src)