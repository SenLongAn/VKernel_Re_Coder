set(TARGET_NAME VKernelEngine) ##< defined variable

file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "*.h") ##< Collect header files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "*.cpp") ##< Collect source files

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES}) ##< Display by directory structure

add_library(${TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES}) ##< Build as a library

# properties, options
set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 17) ###< Set the target to C++17 standard
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "Engine") ##< Display by directory structure
target_compile_options(${TARGET_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->") ##< Enable the strict standard consistency mode
target_compile_options(${TARGET_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/WX->") ##< When compiling C++ code using the MSVC compiler, Disable "Treat warnings as errors"

# Link library, including directory
target_link_libraries(${TARGET_NAME} PUBLIC glfw)
target_link_libraries(${TARGET_NAME} PUBLIC ${vulkan_lib})

target_include_directories(
  ${TARGET_NAME}
  PUBLIC 
  $<BUILD_INTERFACE:${vulkan_include}>
)

target_include_directories(
  ${TARGET_NAME} 
  PUBLIC 
  $<BUILD_INTERFACE:${THIRD_PARTY_DIR}/vulkanmemoryallocator/include>
)

target_include_directories(
  ${TARGET_NAME} 
  PUBLIC 
  $<BUILD_INTERFACE:${ENGINE_ROOT_DIR}/source>
)