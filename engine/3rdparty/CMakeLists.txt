set(third_party_folder "ThirdParty") ##< defined variable

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE) ##< Force all libraries to be built as static libraries

if(NOT TARGET imgui) ##< If there is no target
    include(imgui.cmake) ##< Include and execute the .cmake file
    set_target_properties(imgui PROPERTIES FOLDER ${third_party_folder}/imgui) ##< Display by directory structure
endif()

if(NOT TARGET glfw) ##< If there is no target
    option(GLFW_BUILD_EXAMPLES "" OFF)      ##< No need for examples, GLFW built-in options
    option(GLFW_BUILD_TESTS "" OFF)         ##< No need for tests
    option(GLFW_BUILD_DOCS "" OFF)          ##< No need for documentation
    option(GLFW_INSTALL "" OFF)             ##< No separate installation required
    add_subdirectory(glfw) ##< glfw subdirectories, Have GLFW generate the glfw3.lib library file
    set_target_properties(glfw PROPERTIES FOLDER ${third_party_folder}/glfw) ##< Display by directory structure
    set_target_properties(update_mappings PROPERTIES FOLDER ${third_party_folder}/glfw) ##< Display by directory structure
endif()

if(NOT TARGET stb) ##< If there is no target
    include(stb.cmake) ##< Include and execute the stb.cmake file
    set_target_properties(stb PROPERTIES FOLDER ${third_party_folder}/stb) ##< Display by directory structure
endif()

if(NOT TARGET tinyobjloader) ##< If there is no target
    add_subdirectory(tinyobjloader) ##< Add subdirectories
    set_target_properties(tinyobjloader PROPERTIES FOLDER ${third_party_folder}/tinyobjloader) ##< Display by directory structure
    set_target_properties(uninstall PROPERTIES FOLDER ${third_party_folder}/tinyobjloader) ##< Display by directory structure
endif()

