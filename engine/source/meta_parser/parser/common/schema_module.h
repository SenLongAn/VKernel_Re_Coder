#pragma once
#include "precompiled.h"

/**
 * All Class in a file
 */

class Class;
class Global;
class Function;
class Enum;

struct SchemaMoudle
{
    std::string name;

    std::vector<std::shared_ptr<Class>> classes;
};