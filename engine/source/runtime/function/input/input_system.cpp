#include "runtime/function/input/input_system.h"

#include <GLFW/glfw3.h>

#include "runtime/engine.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_system.h"
#include "runtime/function/render/window_system.h"

#include <iostream>

namespace VKernel
{
    unsigned int k_complement_control_command = 0xFFFFFFFF;

    void InputSystem::initialize()
    {
        // Register to the observer
        std::shared_ptr<WindowSystem> window_system = g_runtime_global_context.m_window_system;

        window_system->registerOnKeyFunc(std::bind(&InputSystem::onKey,
                                                   this,
                                                   std::placeholders::_1,
                                                   std::placeholders::_2,
                                                   std::placeholders::_3,
                                                   std::placeholders::_4));

        window_system->registerOnCursorPosFunc(
            std::bind(&InputSystem::onCursorPos, this, std::placeholders::_1, std::placeholders::_2));
    }

    void InputSystem::tick() { clear(); }

    void InputSystem::clear()
    {
        // clear
        m_cursor_delta_x = 0;
        m_cursor_delta_y = 0;
    }

    void InputSystem::onKey(int key, int scancode, int action, int mods)
    {
        if (g_is_editor_mode)
            return;

        onKeyInGameMode(key, scancode, action, mods);
    }

    void InputSystem::onCursorPos(double current_cursor_x, double current_cursor_y)
    {
        if (g_is_editor_mode)
            return;

        // calculate xy delta
        m_cursor_delta_x = m_last_cursor_x - current_cursor_x;
        m_cursor_delta_y = m_last_cursor_y - current_cursor_y;

        // last xy
        m_last_cursor_x = current_cursor_x;
        m_last_cursor_y = current_cursor_y;
    }

    void InputSystem::onKeyInGameMode(int key, int scancode, int action, int mods)
    {
        m_game_command &= (k_complement_control_command ^ (unsigned int)GameCommand::jump);

        if (action == GLFW_PRESS) ///< press
        {
            switch (key)
            {
                case GLFW_KEY_A:
                    m_game_command |= (unsigned int)GameCommand::left;
                    break;
                case GLFW_KEY_S:
                    m_game_command |= (unsigned int)GameCommand::backward;
                    break;
                case GLFW_KEY_W:
                    m_game_command |= (unsigned int)GameCommand::forward;
                    break;
                case GLFW_KEY_D:
                    m_game_command |= (unsigned int)GameCommand::right;
                    break;
                case GLFW_KEY_SPACE:
                    m_game_command |= (unsigned int)GameCommand::jump;
                    break;
                case GLFW_KEY_LEFT_SHIFT:
                    m_game_command |= (unsigned int)GameCommand::sprint;
                    break;
                case GLFW_KEY_F11:
                    g_is_full_screen_mode = !g_is_full_screen_mode;
                    g_runtime_global_context.m_window_system->showTitleBar(!g_is_full_screen_mode);
                    break;
                case GLFW_KEY_F:
                    m_game_command ^= (unsigned int)GameCommand::first_camera;
                    break;
                default:
                    break;
            }
        }
        else if (action == GLFW_RELEASE) ///< release
        {
            switch (key)
            {
                case GLFW_KEY_W:
                    m_game_command &= (k_complement_control_command ^ (unsigned int)GameCommand::forward);
                    break;
                case GLFW_KEY_S:
                    m_game_command &= (k_complement_control_command ^ (unsigned int)GameCommand::backward);
                    break;
                case GLFW_KEY_A:
                    m_game_command &= (k_complement_control_command ^ (unsigned int)GameCommand::left);
                    break;
                case GLFW_KEY_D:
                    m_game_command &= (k_complement_control_command ^ (unsigned int)GameCommand::right);
                    break;
                case GLFW_KEY_LEFT_SHIFT:
                    m_game_command &= (k_complement_control_command ^ (unsigned int)GameCommand::sprint);
                    break;
                default:
                    break;
            }
        }
    }

} // namespace VKernel