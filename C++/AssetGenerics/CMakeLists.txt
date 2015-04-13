cmake_minimum_required(VERSION 2.8)

project ( AssetGenerics CXX )
file ( GLOB_RECURSE SRC *.?pp )
add_library(AssetGenerics SHARED ${SRC})
target_link_libraries(AssetGenerics Qt5::Core)
target_link_libraries(AssetGenerics Qt5::Gui)
target_link_libraries(AssetGenerics Utility)
target_link_libraries(AssetGenerics AssetInterfaces)
