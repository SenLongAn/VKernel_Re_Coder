#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>

/**
 * window core
 */
namespace VKernel
{
    struct WindowCreateInfo
    {
        int width {500};
        int height {500};
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

        void pollEvents() const; // glfw event
        bool shouldClose() const; ///< Have you closed the window?

    protected:
        // callbacks func
        static void windowSizeCallback(GLFWwindow* window, int width, int height)
        {
            WindowSystem* app = (WindowSystem*)glfwGetWindowUserPointer(window);
            if (app)
            {
                app->m_width  = width;
                app->m_height = height;
            }
        }
        static void windowCloseCallback(GLFWwindow* window) { glfwSetWindowShouldClose(window, true); }

    private:
        GLFWwindow* m_window {nullptr}; ///< window instance
        int         m_width {0}; ///< window width
        int         m_height {0}; ///< window height
    };
}