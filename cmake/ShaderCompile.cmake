# CMake script file

# function definition: 
# func name
# parameters:
# Source shader file list
# Added to SHADER_COMPILE_TARGET
# include path: Data structures shared between the CPU end and the GPU end
# export Folder name
# glslangValidator.exe path

function(compile_shader SHADERS TARGET_NAME SHADER_INCLUDE_FOLDER GENERATED_DIR GLSLANG_BIN) 

    # defined variable
    set(working_dir "${CMAKE_CURRENT_SOURCE_DIR}") ##< The directory where the CMakeLists.txt file is located
    set(ALL_GENERATED_SPV_FILES "")
    set(ALL_GENERATED_CPP_FILES "")

    # Traverse each shader file in the SHADERS list
    foreach(SHADER ${SHADERS})
        # Prepare a header name and a global variable for this shader
        get_filename_component(SHADER_NAME ${SHADER} NAME) ##< SPV: Extract the file name from the complete path to SHADER_NAME
        string(REPLACE "." "_" HEADER_NAME ${SHADER_NAME}) ##< CPP: Replace the old character "." with the new character "_" and output "HEADER_NAME"
        string(TOUPPER ${HEADER_NAME} GLOBAL_SHADER_VAR) ###< converted to uppercase and output "GLOBAL_SHADER_VAR"

        # defined variable, output directory
        set(SPV_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${GENERATED_DIR}/spv/${SHADER_NAME}.spv")
        set(CPP_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${GENERATED_DIR}/cpp/${HEADER_NAME}.h")

        # SPV : OUTPUT - Output file, COMMAND - Command to be executed, DEPENDS - Dependent file, WORKING_DIRECTORY - Working directory
        add_custom_command(
            OUTPUT ${SPV_FILE}
            COMMAND ${GLSLANG_BIN} -I${SHADER_INCLUDE_FOLDER} -V100 -o ${SPV_FILE} ${SHADER} ##< glslangValidator.exe -I include_path -V100(SPIR-V Version 1.0) -o output_file source_file
            DEPENDS ${SHADER}
            WORKING_DIRECTORY "${working_dir}")

        list(APPEND ALL_GENERATED_SPV_FILES ${SPV_FILE}) ##< Add to the list

        # CPP
        add_custom_command(
            OUTPUT ${CPP_FILE}
            COMMAND ${CMAKE_COMMAND} -DPATH=${SPV_FILE} -DHEADER="${CPP_FILE}" 
                -DGLOBAL="${GLOBAL_SHADER_VAR}" -P "${VKERNEL_ROOT_DIR}/cmake/GenerateShaderCPPFile.cmake" ##< Executable_file_path  parameter(-D PATH = SPVPath -D HEADER = CPPPath -D GLOBAL = uppercase), The CMake script to be executed
            DEPENDS ${SPV_FILE}
            WORKING_DIRECTORY "${working_dir}")

        list(APPEND ALL_GENERATED_CPP_FILES ${CPP_FILE}) ##< Add to the list

    endforeach() ##< end foreach

    # add target
    add_custom_target(${TARGET_NAME}
        DEPENDS ${ALL_GENERATED_SPV_FILES} ${ALL_GENERATED_CPP_FILES} SOURCES ${SHADERS})

endfunction() ##< end function