#pragma once

/**
 * Wrapper for CXType
 */

class Cursor;

class CursorType
{
public:
    CursorType(const CXType& handle); ///< Construction

    std::string GetDisplayName(void) const; ///< Return type raw text

    int GetArgumentCount(void) const; ///< Get the number of parameters of a function type

    CursorType GetArgument(unsigned index) const; ///< Get the type of the index parameter of a function

    CursorType GetCanonicalType(void) const; ///< Get the normalized type, the original type of the typedef

    Cursor GetDeclaration(void) const; ///< Return the cursor for a custom type (class/enum)

    CXTypeKind GetKind(void) const; ///< Get type (e.g., bool, void, className)

    bool IsConst(void) const; ///< Whether the type is qualified with const

private:
    CXType m_handle;
};