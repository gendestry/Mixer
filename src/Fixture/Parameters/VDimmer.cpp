#include "LightEngine/Fixture/Parameters/VDimmer.h"

#include "Utils/Colors/Colors.h"   // rgbToHsv / hsvToRgb

namespace LightEngine::Parameters
{
    std::shared_ptr<Parameter> VDimmerParameter::clone() const
    {
        return std::make_shared<VDimmerParameter>(*this);
    }

    void VDimmerParameter::apply()
    {
        if (m_color == nullptr) return;

        // Dim the RGB by scaling Value (brightness) in HSV space, preserving
        // hue and saturation, then write it back.
        Utils::Colors::HSV hsv = Utils::Colors::rgbToHsv(m_color->getColor());
        hsv.v *= m_level;
        m_color->setColor(Utils::Colors::hsvToRgb(hsv));

        // A white component carries no hue/saturation, so scale it linearly.
        m_color->multiplyChannel(Field::W, m_level);
    }
}
