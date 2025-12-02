#include "runtime/function/render/window_system.h"

#include <iostream>

namespace VKernel
{
    WindowSystem::~WindowSystem()
    {
    }

    void WindowSystem::initialize(WindowCreateInfo create_info)
    {
        if (!glfwInit())
        {
            throw std::runtime_error("failed to initialize GLFW");
            return;
        }

        m_width = create_info.width;
        m_height = create_info.height;

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); ///< Vulkan is required.

        m_window = glfwCreateWindow(create_info.width, create_info.height, create_info.title, nullptr, nullptr);
        if (!m_window)
        {
            throw std::runtime_error("failed to create window");
            glfwTerminate();
            return;
        }

        // Setup input callbacks
        glfwSetWindowUserPointer(m_window, this);
        glfwSetWindowSizeCallback(m_window, windowSizeCallback);
        glfwSetWindowCloseCallback(m_window, windowCloseCallback);
        glfwSetKeyCallback(m_window, keyCallback);
    }

    GLFWwindow *WindowSystem::getWindow() const { return m_window; }

    std::array<int, 2> WindowSystem::getWindowSize() const { return std::array<int, 2>({m_width, m_height}); }

    void WindowSystem::pollEvents() const { glfwPollEvents(); }

    bool WindowSystem::shouldClose() const { return glfwWindowShouldClose(m_window); }
}
