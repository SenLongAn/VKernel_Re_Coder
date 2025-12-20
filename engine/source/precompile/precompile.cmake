# Set Variable
set(PRECOMPILE_TOOLS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin") # engine/bin
set (PARSER_INPUT ${CMAKE_BINARY_DIR}/parser_header.h) # build/parser_header.h
set(VKERNEL_PRECOMPILE_PARAMS_IN_PATH "${CMAKE_CURRENT_SOURCE_DIR}/source/precompile/precompile.json.in") # engine/source/precompile/precompile.json.in
set(VKERNEL_PRECOMPILE_PARAMS_PATH "${PRECOMPILE_TOOLS_PATH}/precompile.json") # engine/bin/precompile.json

# Generate precompile.json file
# Read all ${VAR} or @VAR@ variables in the Vkernel_PRECOMPILE_PARAMS_IN_PATH input file, here referring to all header files in the runtime folder
# Will be automatically converted into a ‘；’ separated string
configure_file(${VKERNEL_PRECOMPILE_PARAMS_IN_PATH} ${VKERNEL_PRECOMPILE_PARAMS_PATH})

# Set Variable
# if (CMAKE_HOST_WIN32)
#     set(PRECOMPILE_PRE_EXE)
# 	set(PRECOMPILE_PARSER ${PRECOMPILE_TOOLS_PATH}/VkernelParser.exe)
#     set(sys_include "*") 
# endif()

set(PRECOMPILE_TARGET "VkernelPreCompile")

# # Called first time when building target 
add_custom_target(${PRECOMPILE_TARGET} ALL

# COMMAND
#   ${CMAKE_COMMAND} -E echo "************************************************************* "
# COMMAND
#   ${CMAKE_COMMAND} -E echo "**** [Precompile] BEGIN "
# COMMAND
#   ${CMAKE_COMMAND} -E echo "************************************************************* "

# COMMAND
#     ${PRECOMPILE_PARSER} "${VKERNEL_PRECOMPILE_PARAMS_PATH}"  "${PARSER_INPUT}"  "${ENGINE_ROOT_DIR}/source" ${sys_include} "Vkernel" 0

# COMMAND
#     ${CMAKE_COMMAND} -E echo "+++ Precompile finished +++"
)
