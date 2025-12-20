#pragma once

#include "cursor_type.h"

/**
 * Wrapper for CXCursor
 * Encapsulation of Clang Functions(clang_xxx)
 */

class Cursor
{
public:
    // Type alias
    typedef std::vector<Cursor> List;

    typedef CXCursorVisitor Visitor;

    Cursor(const CXCursor& handle); ///< Construction

    CXCursorKind getKind(void) const; ///< Get cursor type (e.g., class declaration, class definition, struct...)

    std::string getSpelling(void) const;    ///< Get Identifier
    std::string getDisplayName(void) const; ///< Get the complete identifier

    std::string getSourceFile(void) const; ///< File path of the source file where the cursor is located

    bool isDefinition(void) const; ///< Is it a definition rather than a declaration?

    CursorType getType(void) const; ///< Get cursor type

    List getChildren(void) const; ///< Traverse all direct nodes

    void visitChildren(Visitor visitor, void* data = nullptr); ///< Traverse all direct nodes

private:
    CXCursor m_handle; ///< Equivalent to a tree node pointer
};