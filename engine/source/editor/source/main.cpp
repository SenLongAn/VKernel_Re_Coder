#include "editor/include/editor.h"
#include "runtime/engine.h"

#include <filesystem>
#include <iostream>
/**
 * Entry point
 */
int main(int argc, char** argv)
{
    std::filesystem::path executable_path(argv[0]);
    std::filesystem::path config_file_path =
        executable_path.parent_path() / "ReCoderEditor.ini"; ///< build/.../ReCoderEditor.ini
    std::cout << "path:  " << config_file_path << std::endl;

    VKernel::VKernelEngine* engine = new VKernel::VKernelEngine();
    engine->startEngine(config_file_path.generic_string());
    ReCoder::ReCoderEditor* editor = new ReCoder::ReCoderEditor();
    editor->initialize(engine);

    editor->run();

    editor->clear();
    engine->shutdownEngine();

    return 0;
}