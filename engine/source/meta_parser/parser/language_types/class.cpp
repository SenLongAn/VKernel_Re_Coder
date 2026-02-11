#include "common/precompiled.h"

#include "class.h"

BaseClass::BaseClass(const Cursor& cursor) : name(Utils::getTypeNameWithoutNamespace(cursor.getType())) {}

Class::Class(const Cursor& cursor, const Namespace& current_namespace) :
    TypeInfo(cursor, current_namespace), ///< Base class constructor
    m_name(cursor.getDisplayName()),     ///< Get the complete identifier
    m_name_namespace(""), m_qualified_name(Utils::getTypeNameWithoutNamespace(cursor.getType())),
    m_display_name(Utils::getNameWithoutFirstM(m_qualified_name))
{
    for (auto& x : current_namespace)
    {
        m_name_namespace = m_name_namespace + x + "::";
    }
    m_name_namespace = m_name_namespace + cursor.getDisplayName();

    Utils::replaceAll(m_name, " ", "");
    Utils::replaceAll(m_name, "VKernel::", "");

    for (auto& child : cursor.getChildren()) ///< Traverse all direct nodes
    {
        switch (child.getKind())
        {
            case CXCursor_CXXBaseSpecifier: { ///< If it is a base class
                auto base_class = new BaseClass(child);

                m_base_classes.emplace_back(base_class); ///< Create a class and add it to a vector
            }
            break;

            case CXCursor_FieldDecl: ///< If it is a field
                m_fields.emplace_back(new Field(child, current_namespace, this));
                break;

            case CXCursor_CXXMethod: ///< If it is a method
                m_methods.emplace_back(new Method(child, current_namespace, this));
            default:
                break;
        }
    }
}

bool Class::shouldCompile(void) const { return shouldCompileFields() || shouldCompileMethods(); }

bool Class::shouldCompileFields(void) const
{
    return m_meta_data.getFlag(NativeProperty::All) || m_meta_data.getFlag(NativeProperty::Fields) ||
           m_meta_data.getFlag(NativeProperty::WhiteListFields);
}

bool Class::shouldCompileMethods(void) const
{

    return m_meta_data.getFlag(NativeProperty::All) || m_meta_data.getFlag(NativeProperty::Methods) ||
           m_meta_data.getFlag(NativeProperty::WhiteListMethods);
}

std::string Class::getClassName(void) { return m_name; }

std::string Class::getClassNameNameSpace(void) { return m_name_namespace; }

bool Class::isAccessible(void) const { return m_enabled; }