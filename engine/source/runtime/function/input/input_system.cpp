#include "runtime/function/input/input_system.h"

#include "runtime/function/global/global_context.h"
#include "runtime/function/render/window_system.h"

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
    }

    void InputSystem::onKey(int key, int scancode, int action, int mods)
    {
        onKeyInGameMode(key, scancode, action, mods);
    }

    void InputSystem::onKeyInGameMode(int key, int scancode, int action, int mods)
    {
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
                default:
                    break;
            }
        }
    }
}