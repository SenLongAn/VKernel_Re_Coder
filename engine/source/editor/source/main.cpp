#include "runtime/engine.h"
#include "editor/include/editor.h"

/**
 * Entry point
 */
int main()
{
    
    VKernel::VKernelEngine* engine = new VKernel::VKernelEngine();
    engine->startEngine();
    ReCoder::ReCoderEditor* editor = new ReCoder::ReCoderEditor();
    editor->initialize();

    editor->run();

    editor->clear();
    engine->shutdownEngine();
    
    return 0;
}