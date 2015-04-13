cmake_minimum_required(VERSION 2.8)

project ( AssetInterfaces CXX )
file ( GLOB_RECURSE SRC *.?pp )
add_library(AssetInterfaces SHARED "${SRC}")
target_link_libraries(AssetInterfaces Qt5::Core)
target_link_libraries(AssetInterfaces Qt5::Gui)
target_link_libraries(AssetInterfaces Utility)
