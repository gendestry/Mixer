#include "Fixture/Parameters/Channel.h"

namespace Core::Parameters
{
    std::string_view typeName(Type t)
    {
        switch (t)
        {
            case Type::DIMMER:   return "DIMMER";
            case Type::COLOR:    return "COLOR";
            case Type::POSITION: return "POSITION";
            case Type::VDIMMER:  return "VDIMMER";
            default:             return "NONE";
        }
    }

    std::string_view fieldName(Field f)
    {
        switch (f)
        {
            case Field::R:      return "R";
            case Field::G:      return "G";
            case Field::B:      return "B";
            case Field::W:      return "W";
            case Field::A:      return "A";
            case Field::PAN:    return "PAN";
            case Field::TILT:   return "TILT";
            case Field::DIMMER: return "DIMMER";
        }
        return "?";
    }
}
