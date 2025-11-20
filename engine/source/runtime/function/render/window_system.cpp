#include "runtime/function/render/window_system.h"

#include <iostream>

namespace VKernel
{
    WindowSystem::~WindowSystem()
    {
    }

    void WindowSystem::initialize(WindowCreateInfo create_info)
    {
        if(!glfwInit())
        {
            std::cerr << "failed to initialize GLFW" << std::endl;
            return;
        }

        m_width  = create_info.width;
        m_height = create_info.height;

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); ///< Vulkan is required.

        m_window = glfwCreateWindow(create_info.width, create_info.height, create_info.title, nullptr, nullptr);
        if (!m_window)
        {
            std::cerr << "failed to create window" << std::endl;
            glfwTerminate();
            return;
        }
    }

    GLFWwindow* WindowSystem::getWindow() const { return m_window; }

    std::array<int, 2> WindowSystem::getWindowSize() const { return std::array<int, 2>({m_width, m_height}); }
}
