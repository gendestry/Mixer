//
// Created by bobi on 15/09/2025.
//

#pragma once
#include <string>

namespace Traits
{
    struct Printable
    {
        virtual ~Printable() {}
        [[nodiscard]] virtual std::string describe() const = 0;
    };
}
