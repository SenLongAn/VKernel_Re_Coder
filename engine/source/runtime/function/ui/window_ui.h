#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <imgui.h>
#include <imgui_internal.h>

/**
 * Editor UI Settings, Abstract class
 */

namespace VKernel
{
    class WindowSystem;
    class RenderSystem;

    struct WindowUIInitInfo
    {
        std::shared_ptr<WindowSystem> window_system;
        std::shared_ptr<RenderSystem> render_system;
    };

    class WindowUI
    {
    public:
        static bool selected_object; ///< Can the panel be penetrated to select objects?

    public:
        WindowUI() = default;
        WindowUI(bool isGameMode);
        virtual void initialize(WindowUIInitInfo init_info) = 0;
        virtual void preRender()                            = 0;
        bool         iSGameMode() { return m_only_game_mode; }

    protected:
        bool m_only_game_mode = false;
    };

    class WindowUIFactory
    {
    public:
        WindowUIFactory() {}
        using CreatorFunc = std::function<std::shared_ptr<WindowUI>()>; ///< function type: Return value

        static WindowUIFactory& getInstance() ///< Singleton mode
        {
            static WindowUIFactory instance;
            return instance;
        }

        template<typename T> ///< template function
        void registerUI(bool isGameMode)
        {
            m_creators[++index] = [isGameMode]() {
                return std::make_shared<T>(isGameMode);
            }; ///< Register function to m_creators
        }

        // Create derived class
        std::vector<std::shared_ptr<WindowUI>> createUIs()
        {
            std::vector<std::shared_ptr<WindowUI>> res;
            for (auto& x : m_creators)
            {
                res.push_back(x.second());
            }
            return res;
        }

    private:
        std::unordered_map<int, CreatorFunc> m_creators; ///< index, create instance func
        static int                           index;
    };

    // create instance function
#define WINDOWUI_CLASS(ClassName) \
public: \
    static bool _s_registered;

    // Register function to m_creators
#define WINDOWUI_REGISTER(ClassName, isGameMode) \
    bool ClassName::_s_registered = []() { \
        VKernel::WindowUIFactory::getInstance().registerUI<ClassName>(isGameMode); \
        return true; \
    }();

    class WindowUIManager
    {
    public:
        // init
        WindowUIManager() {}
        void registerUIs();
        void initUIs(WindowUIInitInfo init_info);

        // tick
        void updateUIs();

        // clear
        ~WindowUIManager();

        std::vector<std::shared_ptr<WindowUI>> m_uis;
    };
} // namespace VKernel