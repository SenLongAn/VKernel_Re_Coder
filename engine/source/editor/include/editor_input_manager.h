#pragma once

#include "runtime/core/math/vector2.h"

/**
 * Manage user input data
 */

namespace ReCoder
{
    enum class EditorCommand : unsigned int ///< Keyboard key command type, bit mask
    {
        camera_left = 1 << 0,   // A
        camera_back = 1 << 1,   // S
        camera_foward = 1 << 2, // W
        camera_right = 1 << 3,  // D
    };

    class EditorInputManager
    {

    public:
        void initialize();           ///< init
        void tick(float delta_time); ///< tick

        void onKey(int key, int scancode, int action, int mods); ///< Update the key data
        void onCursorPos(double xpos, double ypos);              ///< Update mouse data
        void onScroll(double xoffset, double yoffset);           ///< update mouse data

    private:
        // mouse data
        float m_mouse_x{0.0f};
        float m_mouse_y{0.0f};
        float m_camera_speed{0.05f};

        // key data
        unsigned int m_editor_command{0};

    private:
        void registerInput(); ///< register func

        void processEditorCommand(); ///< process key data

        void onKeyInEditorMode(int key, int scancode, int action, int mods); ///< Update the key data

        bool isCursorInRect(VKernel::Vector2 pos, VKernel::Vector2 size) const; ///< Check whether the cursor is within the window.
    };
}