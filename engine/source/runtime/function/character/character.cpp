#include "runtime/function/character/character.h"

namespace VKernel
{
    CharacterManager::CharacterManager() {}

    std::shared_ptr<Character> CharacterManager::registerCharacter(const std::string& characterType,

                                                                   std::shared_ptr<GObject> characterObject)
    {
        auto character = CharacterFactory::getInstance().createCharacter(characterType, characterObject);

        if (character)
        {
            m_characters.push_back(character);
        }

        return character;
    }

    void CharacterManager::setCurrentCharacter(std::shared_ptr<Character> character)
    {
        Character::setCurrentCharacter(character);
    }

    void CharacterManager::updateCurrentCharacter(float delta_time)
    {
        if (Character::getCurrentCharacter())
        {
            Character::getCurrentCharacter()->tick(delta_time);
        }
    }

    void CharacterManager::clearCharacters() { m_characters.clear(); }

    std::shared_ptr<Character> Character::m_current_character = nullptr;
    std::shared_ptr<Character> Character::getCurrentCharacter() { return m_current_character; }

    void Character::setObject(std::shared_ptr<GObject> gobject) { m_character_object = gobject; }

    GObjectID Character::getObjectID() const
    {
        if (m_character_object)
        {
            return m_character_object->getID();
        }

        return k_invalid_gobject_id;
    }
} // namespace VKernel
