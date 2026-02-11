#pragma once

#include "type_info.h"

#include "field.h"
#include "method.h"

/**
 * Class: Metadata information of the c++ class
 */

struct BaseClass ///< base class
{
    BaseClass(const Cursor& cursor);

    std::string name; ///< Class Type Name
};

class Class : public TypeInfo
{
    // to access m_qualifiedName
    friend class Field;
    friend class Method;
    friend class MetaParser;

public:
    Class(const Cursor& cursor, const Namespace& current_namespace); ///< Construction

    virtual bool shouldCompile(void) const; ///< If there are attributes of fields/methods

    bool shouldCompileFields(void) const;
    bool shouldCompileMethods(void) const;

    template<typename T>
    using SharedPtrVector = std::vector<std::shared_ptr<T>>;

    std::string getClassName(void);
    std::string getClassNameNameSpace(void);

    SharedPtrVector<BaseClass> m_base_classes;

public:
    std::string m_name;           ///< complete identifier
    std::string m_name_namespace; ///< complete identifier

    std::string m_qualified_name; ///< type

    SharedPtrVector<Field>  m_fields;
    SharedPtrVector<Method> m_methods;

    std::string m_display_name; ///< type

    bool isAccessible(void) const; ///< If there are attributes of fields/methods
};
