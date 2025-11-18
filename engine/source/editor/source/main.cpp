#include "runtime/engine.h"
#include "editor/include/editor.h"

/**
 * Entry point
 */
int main(){
    
    VKernel::VKernelEngine* engine = new VKernel::VKernelEngine();
    engine->initialize();
    ReCoder::ReCoderEditor* editor = new ReCoder::ReCoderEditor();
    editor->initialize();

    editor->run();

    engine->clear();
    editor->clear();

    return 0;
}