#include "editor/include/editor_input_manager.h"

#include "editor/include/editor_global_context.h"
#include "editor/include/editor_scene_manager.h"

#include "runtime/function/render/window_system.h"
#include "runtime/function/render/render_system.h"
#include "runtime/function/render/render_camera.h"
#include "runtime/function/input/input_system.h"

#include "runtime/core/math/math_headers.h"

namespace ReCoder
{
    void EditorInputManager::initialize() { registerInput(); }

    void EditorInputManager::tick(float delta_time) { processEditorCommand(); }

    void EditorInputManager::registerInput()
    {
        g_editor_global_context.m_window_system->registerOnCursorPosFunc(
            std::bind(&EditorInputManager::onCursorPos, this, std::placeholders::_1, std::placeholders::_2));
        g_editor_global_context.m_window_system->registerOnKeyFunc(std::bind(&EditorInputManager::onKey,
                                                                             this,
                                                                             std::placeholders::_1,
                                                                             std::placeholders::_2,
                                                                             std::placeholders::_3,
                                                                             std::placeholders::_4));
    }

    void EditorInputManager::onKey(int key, int scancode, int action, int mods)
    {
        onKeyInEditorMode(key, scancode, action, mods);
    }

    void EditorInputManager::onCursorPos(double xpos, double ypos)
    {
        if (g_editor_global_context.m_window_system->isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
        {
            float angularVelocity =
                180.0f / VKernel::Math::max(g_editor_global_context.m_window_system->getWindowSize()[0], g_editor_global_context.m_window_system->getWindowSize()[1]);

            g_editor_global_context.m_render_system->getRenderCamera()->rotate(
                VKernel::Vector2(ypos - m_mouse_y, xpos - m_mouse_x) * angularVelocity);
        }
        m_mouse_x = xpos;
        m_mouse_y = ypos;
    }

    void EditorInputManager::processEditorCommand()
    {
        float camera_speed = m_camera_speed;
        std::shared_ptr editor_camera = g_editor_global_context.m_scene_manager->getEditorCamera();
        VKernel::Quaternion camera_rotate = editor_camera->rotation().inverse();
        VKernel::Vector3 camera_relative_pos(0, 0, 0);

        if ((unsigned int)EditorCommand::camera_foward & m_editor_command)
        {
            camera_relative_pos += camera_rotate * VKernel::Vector3{0, camera_speed, 0};
        }
        if ((unsigned int)EditorCommand::camera_back & m_editor_command)
        {
            camera_relative_pos += camera_rotate * VKernel::Vector3{0, -camera_speed, 0};
        }
        if ((unsigned int)EditorCommand::camera_left & m_editor_command)
        {
            camera_relative_pos += camera_rotate * VKernel::Vector3{-camera_speed, 0, 0};
        }
        if ((unsigned int)EditorCommand::camera_right & m_editor_command)
        {
            camera_relative_pos += camera_rotate * VKernel::Vector3{camera_speed, 0, 0};
        }

        editor_camera->move(camera_relative_pos);
    }

    void EditorInputManager::onKeyInEditorMode(int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS)
        {
            switch (key)
            {
            case GLFW_KEY_A:
                m_editor_command |= (unsigned int)EditorCommand::camera_left;
                break;
            case GLFW_KEY_S:
                m_editor_command |= (unsigned int)EditorCommand::camera_back;
                break;
            case GLFW_KEY_W:
                m_editor_command |= (unsigned int)EditorCommand::camera_foward;
                break;
            case GLFW_KEY_D:
                m_editor_command |= (unsigned int)EditorCommand::camera_right;
                break;
            default:
                break;
            }
        }
        else if (action == GLFW_RELEASE)
        {
            switch (key)
            {
            case GLFW_KEY_A:
                m_editor_command &= (VKernel::k_complement_control_command ^ (unsigned int)EditorCommand::camera_left);
                break;
            case GLFW_KEY_S:
                m_editor_command &= (VKernel::k_complement_control_command ^ (unsigned int)EditorCommand::camera_back);
                break;
            case GLFW_KEY_W:
                m_editor_command &= (VKernel::k_complement_control_command ^ (unsigned int)EditorCommand::camera_foward);
                break;
            case GLFW_KEY_D:
                m_editor_command &= (VKernel::k_complement_control_command ^ (unsigned int)EditorCommand::camera_right);
                break;
            default:
                break;
            }
        }
    }
}
