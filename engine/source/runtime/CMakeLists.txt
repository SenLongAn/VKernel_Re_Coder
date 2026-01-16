set(TARGET_NAME VKernelEngine) ##< defined variable

# json11
set(JSON_INCLUDE ${THIRD_PARTY_DIR}/json11) ##< defined variable
add_library(json11 ${JSON_INCLUDE}/json11.cpp) ##< Build as a library
set_target_properties(json11 PROPERTIES FOLDER "ThirdParty/json11") ##< Display by directory structure

# runtime
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "*.h") ##< Collect header files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "*.cpp") ##< Collect source files

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES}) ##< Display by directory structure

add_library(${TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES}) ##< Build as a library

# properties, options
set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 17) ###< Set the target to C++17 standard
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "Engine") ##< Display by directory structure
target_compile_options(${TARGET_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->") ##< Enable the strict standard consistency mode
target_compile_options(${TARGET_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/WX->") ##< When compiling C++ code using the MSVC compiler, Disable "Treat warnings as errors"

# set global vari used by precompile.json
set(VKERNEL_RUNTIME_HEADS "${HEADER_FILES}" PARENT_SCOPE)

# Link library, including directory
target_link_libraries(${TARGET_NAME} PUBLIC spdlog::spdlog)
target_link_libraries(${TARGET_NAME} PUBLIC glfw)
target_link_libraries(${TARGET_NAME} PUBLIC ${vulkan_lib})
target_link_libraries(${TARGET_NAME} PRIVATE tinyobjloader stb) ##< tinyobjloader and stb
target_link_libraries(${TARGET_NAME} PRIVATE $<BUILD_INTERFACE:json11>)
target_link_libraries(${TARGET_NAME} PUBLIC imgui)

target_include_directories(
  ${TARGET_NAME}
  PUBLIC 
  $<BUILD_INTERFACE:${vulkan_include}> # vulkan
)

target_include_directories(
  ${TARGET_NAME} 
  PUBLIC 
  $<BUILD_INTERFACE:${THIRD_PARTY_DIR}/vulkanmemoryallocator/include> # vulkan memory allocator
)

target_include_directories(
  ${TARGET_NAME} 
  PUBLIC 
  $<BUILD_INTERFACE:${ENGINE_ROOT_DIR}/source> # engine/source
)

target_include_directories(
  ${TARGET_NAME} 
  PUBLIC 
  $<BUILD_INTERFACE:${ENGINE_ROOT_DIR}> # engine/
)

add_dependencies(${TARGET_NAME} ${SHADER_COMPILE_TARGET}) ##< Set Build Order, First compile the shader
target_include_directories(
  ${TARGET_NAME}
  PUBLIC $<BUILD_INTERFACE:${ENGINE_ROOT_DIR}/shader/generated/cpp>) # engine/shader/generated/cpp

target_include_directories(
  ${TARGET_NAME} 
  PUBLIC $<BUILD_INTERFACE:${JSON_INCLUDE}> # json11
)