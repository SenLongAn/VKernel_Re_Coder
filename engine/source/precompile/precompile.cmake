# Set Variable
set(PRECOMPILE_TOOLS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin") # engine/bin
set (PARSER_INPUT ${CMAKE_BINARY_DIR}/parser_header.h) # build/parser_header.h
set(VKERNEL_PRECOMPILE_PARAMS_IN_PATH "${CMAKE_CURRENT_SOURCE_DIR}/source/precompile/precompile.json.in") # engine/source/precompile/precompile.json.in
set(VKERNEL_PRECOMPILE_PARAMS_PATH "${PRECOMPILE_TOOLS_PATH}/precompile.json") # engine/bin/precompile.json

# Generate precompile.json file
# Read all ${VAR} or @VAR@ variables in the VKernel_PRECOMPILE_PARAMS_IN_PATH input file, here referring to all header files in the runtime folder
# Will be automatically converted into a ‘；’ separated string
configure_file(${VKERNEL_PRECOMPILE_PARAMS_IN_PATH} ${VKERNEL_PRECOMPILE_PARAMS_PATH})

# Set Variable
if (CMAKE_HOST_WIN32) # If it's a Windows platform
	set(PRECOMPILE_PARSER ${PRECOMPILE_TOOLS_PATH}/VKernelParser.exe) # engine/bin/VKernelParser.exe
  set(sys_include "*") # Setting it to '*' means 'don't care'
endif()

set(PRECOMPILE_TARGET "VKernelPreCompile")

# Add func
add_custom_target(
  ${PRECOMPILE_TARGET} ALL # all means: the following commands will be executed during the build

  # Text inserted at the beginning
  COMMAND
    ${CMAKE_COMMAND} -E echo "************************************************************* "
  COMMAND
    ${CMAKE_COMMAND} -E echo "**** [Precompile] BEGIN "
  COMMAND
    ${CMAKE_COMMAND} -E echo "************************************************************* "

  # Run VKernelParser.exe
  # engine/bin/VKernelParser.exe
  # engine/bin/precompile.json
  # build/parser_header.h
  # engine/source
  # *, Show indifference
  # VKernel
  # 0
  COMMAND
      ${PRECOMPILE_PARSER} "${VKERNEL_PRECOMPILE_PARAMS_PATH}"  "${PARSER_INPUT}"  "${ENGINE_ROOT_DIR}/source" ${sys_include} "VKernel" 0

  # Text inserted at the end
  COMMAND
      ${CMAKE_COMMAND} -E echo "+++ Precompile finished +++"
)
