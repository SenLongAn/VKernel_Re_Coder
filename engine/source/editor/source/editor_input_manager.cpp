#include "editor/include/editor_input_manager.h"

#include "editor/include/editor_global_context.h"
#include "editor/include/editor_scene_manager.h"

#include "runtime/function/framework/component/transform/transform_component.h"
#include "runtime/function/framework/level/level.h"
#include "runtime/function/framework/world/world_manager.h"
#include "runtime/function/global/global_context.h"
#include "runtime/function/input/input_system.h"
#include "runtime/function/render/render_camera.h"
#include "runtime/function/render/render_system.h"
#include "runtime/function/render/window_system.h"

#include "runtime/core/math/math_headers.h"

#include <iostream>

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
        g_editor_global_context.m_window_system->registerOnScrollFunc(
            std::bind(&EditorInputManager::onScroll, this, std::placeholders::_1, std::placeholders::_2));

        g_editor_global_context.m_window_system->registerOnMouseButtonFunc(
            std::bind(&EditorInputManager::onMouseButtonClicked, this, std::placeholders::_1, std::placeholders::_2));
    }

    void EditorInputManager::onKey(int key, int scancode, int action, int mods)
    {
        onKeyInEditorMode(key, scancode, action, mods);
    }

    void EditorInputManager::onCursorPos(double xpos, double ypos)
    {
        if (g_editor_global_context.m_window_system->isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT)) ///< mouse right
        {
            float angularVelocity =
                180.0f / VKernel::Math::max(g_editor_global_context.m_window_system->getWindowSize()[0],
                                            g_editor_global_context.m_window_system->getWindowSize()[1]);

            g_editor_global_context.m_render_system->getRenderCamera()->rotate(
                VKernel::Vector2(ypos - m_mouse_y, xpos - m_mouse_x) * angularVelocity);
        }
        else if (g_editor_global_context.m_window_system->isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) ///< mouse left
        {
            g_editor_global_context.m_scene_manager->moveEntity(
                xpos,
                ypos,
                m_mouse_x,
                m_mouse_y,
                m_engine_window_pos,
                m_engine_window_size,
                m_cursor_on_axis,
                g_editor_global_context.m_scene_manager->getSelectedObjectMatrix());
        }
        else
        {
            if (isCursorInRect(m_engine_window_pos, m_engine_window_size)) ///< If the mouse is inside the game window
            {
                VKernel::Vector2 cursor_uv =
                    VKernel::Vector2((m_mouse_x - m_engine_window_pos.x) / m_engine_window_size.x,
                                     (m_mouse_y - m_engine_window_pos.y) / m_engine_window_size.y); ///< Calculate uv
                updateCursorOnAxis(cursor_uv); ///< update axis Model matrix
            }
        }
        m_mouse_x = xpos;
        m_mouse_y = ypos;
    }

    void EditorInputManager::onScroll(double xoffset, double yoffset)
    {
        if (isCursorInRect(VKernel::Vector2(0, 0),
                           VKernel::Vector2(g_editor_global_context.m_window_system->getWindowSize()[0],
                                            g_editor_global_context.m_window_system->getWindowSize()[1])))
        {
            if (g_editor_global_context.m_window_system->isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
            {
                if (yoffset > 0)
                {
                    m_camera_speed *= 1.2f;
                }
                else
                {
                    m_camera_speed *= 0.8f;
                }
            }
            else
            {
                g_editor_global_context.m_scene_manager->getEditorCamera()->zoom((float)yoffset * 2.0f);
            }
        }
    }

    void EditorInputManager::onMouseButtonClicked(int key, int action)
    {
        if (m_cursor_on_axis != 3)
            return;

        // Are there any active levels?
        std::shared_ptr<VKernel::Level> current_active_level =
            VKernel::g_runtime_global_context.m_world_manager->getCurrentActiveLevel().lock();
        if (current_active_level == nullptr)
            return;

        if (isCursorInRect(m_engine_window_pos, m_engine_window_size)) ///< Is it in the game window?
        {
            if (key == GLFW_MOUSE_BUTTON_LEFT) ///< click left button
            {
                // uv: 0--1
                VKernel::Vector2 picked_uv((m_mouse_x - m_engine_window_pos.x) / m_engine_window_size.x,
                                           (m_mouse_y - m_engine_window_pos.y) / m_engine_window_size.y);

                // get uv summesh guid
                size_t select_mesh_id = g_editor_global_context.m_scene_manager->getGuidOfPickedMesh(picked_uv);

                // get object id
                size_t gobject_id = g_editor_global_context.m_render_system->getGObjectIDByMeshID(select_mesh_id);

                // update object
                g_editor_global_context.m_scene_manager->onGObjectSelected(gobject_id);
            }
        }
    }

    void EditorInputManager::updateCursorOnAxis(VKernel::Vector2 cursor_uv)
    {
        if (g_editor_global_context.m_scene_manager->getEditorCamera())
        {
            VKernel::Vector2 window_size(m_engine_window_size.x, m_engine_window_size.y);
            m_cursor_on_axis = g_editor_global_context.m_scene_manager->updateCursorOnAxis(cursor_uv, window_size);
        }
    }

    void EditorInputManager::processEditorCommand()
    {
        float               camera_speed  = m_camera_speed;
        std::shared_ptr     editor_camera = g_editor_global_context.m_scene_manager->getEditorCamera();
        VKernel::Quaternion camera_rotate = editor_camera->rotation().inverse();
        VKernel::Vector3    camera_relative_pos(0, 0, 0);

        if ((unsigned int)EditorCommand::camera_foward & m_editor_command)
        {
            camera_relative_pos += camera_rotate * VKernel::Vector3 {0, camera_speed, 0};
        }
        if ((unsigned int)EditorCommand::camera_back & m_editor_command)
        {
            camera_relative_pos += camera_rotate * VKernel::Vector3 {0, -camera_speed, 0};
        }
        if ((unsigned int)EditorCommand::camera_left & m_editor_command)
        {
            camera_relative_pos += camera_rotate * VKernel::Vector3 {-camera_speed, 0, 0};
        }
        if ((unsigned int)EditorCommand::camera_right & m_editor_command)
        {
            camera_relative_pos += camera_rotate * VKernel::Vector3 {camera_speed, 0, 0};
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
                    m_editor_command &=
                        (VKernel::k_complement_control_command ^ (unsigned int)EditorCommand::camera_left);
                    break;
                case GLFW_KEY_S:
                    m_editor_command &=
                        (VKernel::k_complement_control_command ^ (unsigned int)EditorCommand::camera_back);
                    break;
                case GLFW_KEY_W:
                    m_editor_command &=
                        (VKernel::k_complement_control_command ^ (unsigned int)EditorCommand::camera_foward);
                    break;
                case GLFW_KEY_D:
                    m_editor_command &=
                        (VKernel::k_complement_control_command ^ (unsigned int)EditorCommand::camera_right);
                    break;
                default:
                    break;
            }
        }
    }

    bool EditorInputManager::isCursorInRect(VKernel::Vector2 pos, VKernel::Vector2 size) const
    {
        return pos.x <= m_mouse_x && m_mouse_x <= pos.x + size.x && pos.y <= m_mouse_y && m_mouse_y <= pos.y + size.y;
    }
} // namespace ReCoder
