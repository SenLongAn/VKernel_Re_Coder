set(TARGET_NAME VKernelParser) # Set Variable

set_property(GLOBAL PROPERTY USE_FOLDERS ON) # Enable Folder Organization Feature in the IDE

# Recursively get all files and store them in a variable
file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE SOURCES "*.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADERS} ${SOURCES}) # IDE folder organization

# Set Variable
# engine/meta_paser/3rd_party/LLVM/include, engine/meta_paser/3rd_party/mustachem , engine/meta_paser/
set(LLVM_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/LLVM/include ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/mustache ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${ENGINE_ROOT_DIR}/bin) # engine/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${ENGINE_ROOT_DIR}/bin) # engine/bin

include_directories(${LLVM_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/parser) # add LLVM includes： engine/meta_paser/paser

add_executable(${TARGET_NAME} ${HEADERS} ${SOURCES}) # Build as an executable file

# Set target properties
set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 17) # c++17
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "Tools") # IDE folder organization

# Set variables according to the platform
if (CMAKE_HOST_WIN32) # windows
    set(LLVM_LIBRARY_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/LLVM/lib/x64) # engine/meta_paser/3rd_party/LLVM/lib/x64
    set(LLVM_SHARED_LIBRARY_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/LLVM/bin/x64)# engine/meta_paser/3rd_party/LLVM/bin/x64
    target_link_libraries(${TARGET_NAME}  ${LLVM_LIBRARY_DIR}/libclang.lib)# link lib: engine/meta_paser/3rd_party/LLVM/lib/x64/libclang.lib
endif()

# copy resources on post build
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD # POST_BUILD means executing after the target build is completed
	# mustache templates directory
	COMMAND ${CMAKE_COMMAND} -E copy_directory # -E cross-platform, copy_directory command to copy directories
		"${LLVM_SHARED_LIBRARY_DIR}" # Source Directory: engine/meta_paser/3rd_party/LLVM/bin/x64
		$<TARGET_FILE_DIR:${TARGET_NAME}> # Target Directory: The directory where the executable file is located
)

