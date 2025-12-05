#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <array>
#include <functional>
#include <vector>

/**
 * window core
 */
namespace VKernel
{
    struct WindowCreateInfo
    {
        int width{640};
        int height{400};
        const char *title{"VKernel"};
    };

    class WindowSystem ///< window core
    {

    public:
        WindowSystem() = default; ///< Constructor
        ~WindowSystem();          ///< Destructor

        void initialize(WindowCreateInfo create_info); ///< init

        GLFWwindow *getWindow() const;            ///< Get Window
        std::array<int, 2> getWindowSize() const; ///< Get the window size

        void pollEvents() const;  // glfw event
        bool shouldClose() const; ///< Have you closed the window

        // type function
        typedef std::function<void(int, int, int, int)> onKeyFunc;
        typedef std::function<void(double, double)> onCursorPosFunc;
        typedef std::function<void(double, double)> onScrollFunc;

        // Register to the observer
        void registerOnKeyFunc(onKeyFunc func) { m_onKeyFunc.push_back(func); }
        void registerOnCursorPosFunc(onCursorPosFunc func) { m_onCursorPosFunc.push_back(func); }
        void registerOnScrollFunc(onScrollFunc func) { m_onScrollFunc.push_back(func); }

        bool isMouseButtonDown(int button) const ///< press the button?
        {
            if (button < GLFW_MOUSE_BUTTON_1 || button > GLFW_MOUSE_BUTTON_LAST)
            {
                return false;
            }
            return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
        }

    protected:
        // callbacks func
        static void windowSizeCallback(GLFWwindow *window, int width, int height)
        {
            WindowSystem *app = (WindowSystem *)glfwGetWindowUserPointer(window);
            if (app)
            {
                app->m_width = width;
                app->m_height = height;
            }
        }
        static void windowCloseCallback(GLFWwindow *window) { glfwSetWindowShouldClose(window, true); }

        static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
        {
            WindowSystem *app = (WindowSystem *)glfwGetWindowUserPointer(window);
            if (app)
            {
                app->onKey(key, scancode, action, mods);
            }
        }

        static void cursorPosCallback(GLFWwindow *window, double xpos, double ypos)
        {
            WindowSystem *app = (WindowSystem *)glfwGetWindowUserPointer(window);
            if (app)
            {
                app->onCursorPos(xpos, ypos);
            }
        }

        static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
        {
            WindowSystem *app = (WindowSystem *)glfwGetWindowUserPointer(window);
            if (app)
            {
                app->onScroll(xoffset, yoffset);
            }
        }

        void onKey(int key, int scancode, int action, int mods)
        {
            for (auto &func : m_onKeyFunc)
                func(key, scancode, action, mods);
        }

        void onCursorPos(double xpos, double ypos)
        {
            for (auto &func : m_onCursorPosFunc)
                func(xpos, ypos);
        }

        void onScroll(double xoffset, double yoffset)
        {
            for (auto &func : m_onScrollFunc)
                func(xoffset, yoffset);
        }

    private:
        GLFWwindow *m_window{nullptr}; ///< window instance
        int m_width{0};                ///< window width
        int m_height{0};               ///< window height

        // List of Observers
        std::vector<onKeyFunc> m_onKeyFunc;
        std::vector<onCursorPosFunc> m_onCursorPosFunc;
        std::vector<onScrollFunc> m_onScrollFunc;
    };
}