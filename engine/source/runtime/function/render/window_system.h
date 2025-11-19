#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>

namespace VKernel
{
    struct WindowCreateInfo
    {
        int width {1920};
        int height {1080};
        const char* title {"VKernel"};
    };

    class WindowSystem ///< window core
    {

    public:
        WindowSystem() = default; ///< Constructor
        ~WindowSystem(); ///< Destructor

        void initialize(WindowCreateInfo create_info); ///< init

        GLFWwindow* getWindow() const; ///< Get Window
        std::array<int, 2> getWindowSize() const; ///< Get the window size

    private:
        GLFWwindow* m_window {nullptr}; ///< window instance
        int         m_width {0}; ///< window width
        int         m_height {0}; ///< window height
    };
}