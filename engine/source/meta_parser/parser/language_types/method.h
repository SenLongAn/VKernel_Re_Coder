#pragma once

#include "type_info.h"

/**
 * Method: Metadata information of the c++ class member Methods
 */

class Class;

class Method : public TypeInfo
{

public:
    Method(const Cursor& cursor, const Namespace& current_namespace, Class* parent = nullptr); ///< Construction

    virtual ~Method(void) {}

    bool shouldCompile(void) const; ///< Are there any attributes related to the Method?

public:
    Class* m_parent;

    std::string m_name;

    bool isAccessible(void) const; ///< Are there any attributes related to the Method?
};