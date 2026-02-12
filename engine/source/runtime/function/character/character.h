#pragma once

#include "runtime/core/math/transform.h"
#include "runtime/function/framework/object/object.h"

#include <iostream>
#include <string>
/**
 * Character: Player Base Class: Update the player's position and rotation
 * characterManager: Player Manager: At any given moment, only one player will be updated
 */
namespace VKernel
{
    class Character
    {
    public:
        virtual void tick(float delta_time) {}; ///< tick

        // set
        void         setObject(std::shared_ptr<GObject> gobject);
        virtual void setCurrentCharacter() {};

        // get
        GObjectID                         getObjectID() const;
        std::weak_ptr<GObject>            getObject() const { return m_character_object; }
        static std::shared_ptr<Character> getCurrentCharacter();

    protected:
        std::shared_ptr<GObject> m_character_object; ///< object

        static std::shared_ptr<Character> m_current_character;
    };

    class CharacterFactory
    {
    public:
        using CreatorFunc = std::function<std::shared_ptr<Character>(
            std::shared_ptr<GObject>)>; ///< function type: Return value, parameter

        static CharacterFactory& getInstance() ///< Singleton mode
        {
            static CharacterFactory instance;
            return instance;
        }

        template<typename T> ///< template function
        void registerCharacterType(const std::string& typeName)
        {
            m_creators[typeName] = [](std::shared_ptr<GObject> obj) {
                return std::make_shared<T>(obj);
            }; ///< Register function to m_creators
        }

        // Create a derived class based on typeName
        std::shared_ptr<Character> createCharacter(const std::string& typeName, std::shared_ptr<GObject> obj)
        {
            auto it = m_creators.find(typeName);
            if (it != m_creators.end())
            {
                return it->second(obj);
            }
            return nullptr;
        }

    private:
        std::unordered_map<std::string, CreatorFunc> m_creators; ///< class type name, create instance func
    };

    // create instance function
#define CHARACTER_CLASS(ClassName) \
public: \
    static bool _s_registered; \
\
public: \
    static std::shared_ptr<Character> create(std::shared_ptr<VKernel::GObject> obj) \
    { \
        return std::make_shared<ClassName>(obj); \
    }

    // Register function to m_creators
#define CHARACTER_REGISTER(ClassName) \
    bool ClassName::_s_registered = []() { \
        VKernel::CharacterFactory::getInstance().registerCharacterType<ClassName>(#ClassName); \
        return true; \
    }();

    class CharacterManager
    {
    public:
        // init
        CharacterManager();
        std::shared_ptr<Character> registerCharacter(const std::string&       characterType,
                                                     std::shared_ptr<GObject> characterObject);

        // tick
        void                       setCurrentCharacter();
        void                       updateCurrentCharacter(float delta_time);
        std::shared_ptr<Character> getCurrentCharacter() { return Character::getCurrentCharacter(); }

        // clear
        void clearCharacters();
        ~CharacterManager() { clearCharacters(); }

    private:
        std::vector<std::shared_ptr<Character>> m_characters;
    };
} // namespace VKernel