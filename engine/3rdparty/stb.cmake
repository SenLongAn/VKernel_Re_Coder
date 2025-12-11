file(GLOB stb_sources CONFIGURE_DEPENDS  "${CMAKE_CURRENT_SOURCE_DIR}/stb/*.h") # Collect all header files in the stb directory
add_library(stb INTERFACE ${stb_sources}) # Create an interface library named stb and add the collected header files to the library.
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/stb) # Set the include directory for the stb library