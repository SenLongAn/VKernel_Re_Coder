#pragma once

#include "type_info.h"

/**
 * Field: Metadata information of the c++ class member variable
 */

class Class;

class Field : public TypeInfo
{

public:
    Field(const Cursor& cursor, const Namespace& current_namespace, Class* parent = nullptr); ///< Construction

    virtual ~Field(void) {}

    bool shouldCompile(void) const; ///< Are there any attributes related to the field?

public:
    bool m_is_const;

    Class* m_parent;

    std::string m_name;
    std::string m_display_name;
    std::string m_type;

    std::string m_default;

    bool isAccessible(void) const; ///< Are there any attributes related to the field?
};