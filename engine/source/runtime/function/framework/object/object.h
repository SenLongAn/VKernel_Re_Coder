#pragma once

#include "runtime/function/framework/component/component.h"
#include "runtime/resource/res_type/common/object.h"

#include "runtime/function/framework/object/object_id_allocator.h"

#include <memory>

/**
 * Game Object
 */
namespace VKernel
{
    class Component;

    class GObject : public std::enable_shared_from_this<GObject> ///< weak_from_this
    {

    public:
        GObject(GObjectID id) : m_id {id} {}
        virtual ~GObject(); ///< Destructor

        void setName(std::string name) { m_name = name; }

        bool load(const ObjectInstanceRes& object_instance_res); ///< load

        virtual void tick(float delta_time); ///< tick

        GObjectID getID() const { return m_id; } ///< get guid

        void save(ObjectInstanceRes& out_object_instance_res); ///< save

        // Return if there is a specific component
        bool hasComponent(const std::string& compenent_type_name) const;

        template<typename TComponent>
        TComponent* tryGetComponent(const std::string& compenent_type_name)
        {
            for (auto& component : m_components)
            {
                if (component.getTypeName() == compenent_type_name)
                {
                    return static_cast<TComponent*>(component.operator->());
                }
            }

            return nullptr;
        }
        template<typename TComponent>
        const TComponent* tryGetComponentConst(const std::string& compenent_type_name) const
        {
            for (const auto& component : m_components)
            {
                if (component.getTypeName() == compenent_type_name)
                {
                    return static_cast<const TComponent*>(component.operator->());
                }
            }
            return nullptr;
        }

#define tryGetComponent(COMPONENT_TYPE, STR_COMPONENT_TYPE) \
    tryGetComponent<COMPONENT_TYPE>(STR_COMPONENT_TYPE) ///< func call
#define tryGetComponentConst(COMPONENT_TYPE) tryGetComponentConst<const COMPONENT_TYPE>(#COMPONENT_TYPE)

    protected:
        GObjectID m_id {k_invalid_gobject_id};

        std::string m_name;

        std::string m_definition_url;

        std::vector<Reflection::ReflectionPtr<Component>> m_components; ///< component
    };
} // namespace VKernel