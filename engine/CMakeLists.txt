## set directory
set(ENGINE_ROOT_DIR "${VKERNEL_ROOT_DIR}/engine") ##< engine root directory
set(THIRD_PARTY_DIR "${ENGINE_ROOT_DIR}/3rdparty") ##< Third-party database directory
set(ENGINE_ASSET_DIR "/asset") ##< asset directory
set(vulkan_include ${THIRD_PARTY_DIR}/VulkanSDK/include) ##< vulkan include directory
if(WIN32) ##< Windows platform
  set(vulkan_lib ${THIRD_PARTY_DIR}/VulkanSDK/lib/Win32/vulkan-1.lib) ##< vulkan lib directory
  set(glslangValidator_executable ${THIRD_PARTY_DIR}/VulkanSDK/bin/Win32/glslangValidator.exe) ##< Tool for compiling GLSL into bytes directory
else()
  message(FATAL_ERROR "Unknown Platform")
endif()

# properties, options
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") ##< Compiler identifier == MSVC
  add_compile_options("/MP") ##< parallel compiling
  set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ReCoderEditor) ##< Default startup project
endif()

# Add subdirectories
add_subdirectory(source/runtime)
add_subdirectory(source/editor)