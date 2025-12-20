#pragma once
#include "common/namespace.h"

#include "cursor/cursor.h"

#include "meta/meta_info.h"
#include "parser/parser.h"

/**
 * Metadata information of the c++, basic class
 * language_types: refers to the C language types that need to be processed by the reflection system, usually including
 * classes, fields, and methods.
 */

class TypeInfo
{
public:
    TypeInfo(const Cursor& cursor, const Namespace& current_namespace); ///< Construction
    virtual ~TypeInfo(void) {}                                          ///< Destruct

    // get
    const MetaInfo& getMetaData(void) const; ///< m_meta_data

    std::string getSourceFile(void) const; ///< File path of the source file where the m_root_cursor is located

    Namespace getCurrentNamespace() const; ///< m_namespace

    Cursor& getCurosr(); ///< m_root_cursor

protected:
    MetaInfo m_meta_data; ///< Metadata and collection of direct child node attributes

    bool m_enabled; ///< Is there an 'enable' attribute?

    std::string m_alias_cn;

    Namespace m_namespace; ///< The namespace where the cursor is located

private:
    Cursor m_root_cursor; ///< Current cursor
};