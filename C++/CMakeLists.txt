cmake_minimum_required(VERSION 2.8)

set (CMAKE_OSX_ARCHITECTURES "x86_64")

project ( Mif39 CXX )

list ( APPEND CMAKE_PREFIX_PATH "/Locals/Applications/Qt5.4.1/5.4/clang_64/lib/cmake")
find_package(Qt5Core)
find_package(Qt5Gui)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories ( "." )

add_subdirectory(Utility)
add_subdirectory(Networking)
add_subdirectory(TcpNetworking)
add_subdirectory(AssetInterfaces)
add_subdirectory(AssetGenerics)
add_subdirectory(Assets)

add_subdirectory(Wavefront)

add_subdirectory(FuzzyLogicBehaviour)

add_subdirectory(QImageLoader)

add_subdirectory(Samples)
