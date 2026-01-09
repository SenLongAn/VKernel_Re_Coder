#pragma once

#include <memory>
#include <string>

/**
 * Manage all systems and managers
 */
namespace VKernel
{

    class FileSystem;
    class AssetManager;
    class ConfigManager;
    class InputSystem;
    class WorldManager;
    class WindowSystem;
    class RenderSystem;
    class DebugDrawManager;

    class RuntimeGlobalContext ///< Management system and manager
    {

    public:
        // systems and managers
        /*
        It cannot be an ordinary object because we want to maintain global uniqueness. Ordinary objects would result in
        the creation of multiple instances; It cannot be a const object because we need to access/modify its members; It
        cannot use a reference because pointers have more flexible initialization times; It cannot use unique_ptr
        because multiple classes need to share it; It cannot use a regular pointer because shared_ptr has automatic
        lifecycle management, which is more convenient.
        */
        std::shared_ptr<FileSystem>       m_file_system;
        std::shared_ptr<InputSystem>      m_input_system;
        std::shared_ptr<WorldManager>     m_world_manager;
        std::shared_ptr<WindowSystem>     m_window_system;
        std::shared_ptr<RenderSystem>     m_render_system;
        std::shared_ptr<DebugDrawManager> m_debugdraw_manager;
        std::shared_ptr<AssetManager>     m_asset_manager;
        std::shared_ptr<ConfigManager>    m_config_manager;

    public:
        void startSystems(const std::string& config_file_path); ///< Start all systems
        void shutdownSystems();                                 ///< shutdown all System
    };

    extern RuntimeGlobalContext g_runtime_global_context;
} // namespace VKernel