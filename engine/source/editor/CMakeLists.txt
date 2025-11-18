set(TARGET_NAME ReCoderEditor) ##< defined variable

file(GLOB EDITOR_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h) ##< Collect header files
file(GLOB EDITOR_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp) ##< Collect source files

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${EDITOR_HEADERS} ${EDITOR_SOURCES}) ##< Display by directory structure

add_executable(${TARGET_NAME} ${EDITOR_HEADERS} ${EDITOR_SOURCES}) ##< Build as an executable file

# properties, options
set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "ReCoderEditor") ##< Executable file name
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "Engine") ##< Display by directory structure
target_compile_options(${TARGET_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/WX->") ##< When compiling C++ code using the MSVC compiler, Disable "Treat warnings as errors"

target_link_libraries(${TARGET_NAME} VKernelEngine) ##< Link Library