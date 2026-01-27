#pragma once
#include <string>

namespace Traits
{
    struct Serializable
    {
        virtual std::string toString() const = 0;
    };
}