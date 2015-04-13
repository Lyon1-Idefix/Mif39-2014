cmake_minimum_required(VERSION 2.8)

project ( Networking CXX )
file ( GLOB_RECURSE SRC *.?pp )
add_library(Networking SHARED "${SRC}")
target_link_libraries(Networking Qt5::Core)
target_link_libraries(Networking Qt5::Gui)
target_link_libraries(Networking Utility)
