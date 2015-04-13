cmake_minimum_required(VERSION 2.8)

project ( Utility CXX )
file ( GLOB_RECURSE SRC *.?pp )
add_library(Utility SHARED "${SRC}")
target_link_libraries(Utility Qt5::Core)
target_link_libraries(Utility Qt5::Gui)
