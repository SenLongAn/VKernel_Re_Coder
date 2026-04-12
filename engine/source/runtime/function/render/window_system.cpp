#include "runtime/function/render/window_system.h"

#include "runtime/core/base/macro.h"

#include <iostream>

#include <stb_image.h>

#include <windows.h>

namespace VKernel
{
    WindowSystem::~WindowSystem()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void WindowSystem::initialize(WindowCreateInfo create_info)
    {
        if (!glfwInit())
        {
            LOG_ERROR("failed to initialize GLFW");
            return;
        }

        m_width = create_info.width;
        m_height = create_info.height;

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); ///< Do not use OpenGL，Vulkan is required.

        m_window = glfwCreateWindow(create_info.width, create_info.height, create_info.title, nullptr, nullptr);
        if (!m_window)
        {
            LOG_ERROR("failed to create window");
            glfwTerminate();
            return;
        }

        // icon
        setWindowIcon(m_window, "asset/texture/global/icon.ico");

        // Setup input callbacks
        glfwSetWindowUserPointer(m_window, this);
        glfwSetWindowSizeCallback(m_window, windowSizeCallback);
        glfwSetWindowCloseCallback(m_window, windowCloseCallback);
        glfwSetKeyCallback(m_window, keyCallback);
        glfwSetCursorPosCallback(m_window, cursorPosCallback);
        glfwSetScrollCallback(m_window, scrollCallback);
        glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
    }

    void WindowSystem::setWindowIcon(GLFWwindow *window, const char *iconPath)
    {
        HWND hwnd = glfwGetWin32Window(m_window);
        HICON hIcon = (HICON)LoadImage(
            NULL,
            iconPath,
            IMAGE_ICON,
            32, 32,
            LR_LOADFROMFILE);

        if (hIcon)
        {
            PostMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            HICON hSmall = (HICON)LoadImage(NULL, iconPath,
                                            IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
            PostMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hSmall);
        }
    }

    GLFWwindow *WindowSystem::getWindow() const { return m_window; }

    std::array<int, 2> WindowSystem::getWindowSize() const { return std::array<int, 2>({m_width, m_height}); }

    void WindowSystem::pollEvents() const { glfwPollEvents(); }

    bool WindowSystem::shouldClose() const { return glfwWindowShouldClose(m_window); }

    void WindowSystem::setFocusMode(bool mode)
    {
        m_is_focus_mode = mode;
        glfwSetInputMode(m_window, GLFW_CURSOR, m_is_focus_mode ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }

    void WindowSystem::showTitleBar(bool is_show_title_bar)
    {
        if (is_show_title_bar)
        {
            glfwSetWindowAttrib(m_window, GLFW_DECORATED, GLFW_TRUE);
        }
        else
        {
            glfwSetWindowAttrib(m_window, GLFW_DECORATED, GLFW_FALSE);
        }
    }
} // namespace VKernel
