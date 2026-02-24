#pragma once

#include "runtime/core/math/math_angle.h"
#include "runtime/core/math/vector2.h"
#include "runtime/core/math/vector3.h"

/**
 * Manage user input data
 */
namespace VKernel
{
    enum class GameCommand : unsigned int ///< Keyboard key command type, bit mask
    {
        forward      = 1 << 0,                 // W
        backward     = 1 << 1,                 // S
        left         = 1 << 2,                 // A
        right        = 1 << 3,                 // D
        jump         = 1 << 4,                 // jump
        sprint       = 1 << 5,                 // sprint
        first_camera = 1 << 6,                 // first camera
        invalid      = (unsigned int)(1 << 31) // lost focus
    };

    extern unsigned int k_complement_control_command; ///< Used for closing the command

    class InputSystem
    {

    public:
        // current mouse data
        double m_cursor_delta_x {0};
        double m_cursor_delta_y {0};

    public:
        void initialize(); ///< init
        void tick();       ///< tick
        void clear();      ///< clear

        void onKey(int key, int scancode, int action, int mods);            ///< Update the key data
        void onCursorPos(double current_cursor_x, double current_cursor_y); ///< Update mouse data

        unsigned int getGameCommand() const { return m_game_command; } ///< get

        void resetGameCommand() { m_game_command = 0; }

    private:
        unsigned int m_game_command {0}; ///< Current key data

        // last mouse data
        double m_last_cursor_x {0};
        double m_last_cursor_y {0};

        void onKeyInGameMode(int key, int scancode, int action, int mods); ///< Update the key data
    };
} // namespace VKernel