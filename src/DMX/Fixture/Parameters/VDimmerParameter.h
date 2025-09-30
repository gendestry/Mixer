//
// Created by bobi on 27/09/2025.
//

#pragma once
#include <list>
#include <memory>

#include "Parameter.h"

namespace DMX::Parameters
{
    struct VDimmerParameter : public Parameter
    {

        VDimmerParameter(uint8_t* bytes, uint32_t baseOffset): Parameter(nullptr, 0, 0, Type::VDIMMER, 0)
        {
            m_type = Type::VDIMMER;
        }




    };
}
