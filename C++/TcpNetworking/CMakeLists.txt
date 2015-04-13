cmake_minimum_required(VERSION 2.8)

project ( TcpNetworking CXX )
file ( GLOB_RECURSE SRC *.?pp )
add_library(TcpNetworking SHARED ${SRC})
target_link_libraries(TcpNetworking Qt5::Core)
target_link_libraries(TcpNetworking Qt5::Gui)
target_link_libraries(TcpNetworking Utility)
target_link_libraries(TcpNetworking Networking)
