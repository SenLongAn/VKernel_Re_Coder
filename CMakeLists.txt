cmake_minimum_required(VERSION 3.19 FATAL_ERROR) ##< Lowest version

project(VKernel_Recoder VERSION 0.1.0) ##< Project name, project version

set(CMAKE_CXX_STANDARD 17) ##< C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON) ##< If the C++ standard is not supported, an error will be reported.
set(BUILD_SHARED_LIBS OFF) ##< OFF has built all the libraries as static libraries.

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR) ##< If the source directory == the build directory
    message(FATAL_ERROR "In-source builds not allowed...") ##< Print error message
endif()

## set directory
set(VKERNEL_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}") ##< Project root directory
set(BINARY_ROOT_DIR "${CMAKE_INSTALL_PREFIX}/") ##< binary file root directory

add_subdirectory(engine) ##< Add subdirectories