#pragma once
#include <string>
#include <sstream>

namespace Traits 
{
    struct Printable
    {
        [[nodiscard]] virtual std::string describe() const noexcept = 0;
    };
};