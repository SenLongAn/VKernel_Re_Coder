# CMake script file
# Convert the binary file to a C++ header file
# The parameters for the CMake script to be executed: resource file, output source file, variable name

function(embed_resource resource_file_name source_file_name variable_name)

    # If the output file already exists, there is no need to regenerate it.
    if(EXISTS "${source_file_name}")
        if("${source_file_name}" IS_NEWER_THAN "${resource_file_name}")
            return()
        endif()
    endif()

    # generate 
    if(EXISTS "${resource_file_name}")
        file(READ "${resource_file_name}" hex_content HEX) ##< Read the content of the resource file as a hexadecimal string

        string(REPEAT "[0-9a-f]" 32 pattern)
        string(REGEX REPLACE "(${pattern})" "\\1\n" content "${hex_content}") ##< Insert a line break every 32 hexadecimal characters

        string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1, " content "${content}") ##< Add the C++ hexadecimal prefix 0x

        string(REGEX REPLACE ", $" "" content "${content}") ##< Remove the trailing comma and space in the last line.

        set(array_definition "static const std::vector<unsigned char> ${variable_name} =\n{\n${content}\n};") ##< C++ array definition
        
        get_filename_component(file_name ${source_file_name} NAME)
        set(source "/**\n * @file ${file_name}\n * @brief Auto generated file.\n */\n#include <vector>\n${array_definition}\n") ##< Comments and include

        file(WRITE "${source_file_name}" "${source}") ##< Write the generated content into the output file
    else()
        message("ERROR: ${resource_file_name} doesn't exist")
        return()
    endif()

endfunction()

# let's use it as a script
if(EXISTS "${PATH}")
    embed_resource("${PATH}" "${HEADER}" "${GLOBAL}")
endif()
