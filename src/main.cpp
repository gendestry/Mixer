// //
// // Created by bobi on 10/09/2025.
// //
//
// #include <iostream>
// #include <cstdint>
// #include <string>
// #include <unordered_map>
// #include <vector>
// #include <memory>
//
// enum class ColorType
// {
//     R, G, B
// };
//
//
// struct ParameterHelper
// {
//     static constexpr std::string get(ColorType t)
//     {
//         switch (t)
//         {
//         case ColorType::R:
//             return "R";
//         case ColorType::G:
//             return "G";
//         case ColorType::B:
//             return "B";
//         default:
//             return "";
//         }
//     }
// };
// enum class ParameterTypes
// {
//     NONE,
//     DIMMER,
//     COLOR,
//     POSITION,
// };
//
//
// class IfaceParameter
// {
// protected:
// // public:
//     std::unordered_map<std::string, uint16_t> m_offsets;
//     ParameterTypes m_type;
//
// public:
//     IfaceParameter() : m_type(ParameterTypes::NONE) {};
//     explicit IfaceParameter(ParameterTypes type) : m_type(type) {}
//     virtual ~IfaceParameter() = default;
//
//     [[nodiscard]] virtual uint16_t getOffset(ColorType type) const = 0;
//     [[nodiscard]] ParameterTypes getType() const { return m_type; }
//     [[nodiscard]] const std::unordered_map<std::string, uint16_t>& getOffsets() const { return m_offsets; }
// };
//
// template<uint8_t TBytes, ColorType... TTypes>
// struct Color : public IfaceParameter
// {
//     static constexpr std::size_t k_size = sizeof...(TTypes) * TBytes;
//
//     Color() : IfaceParameter(ParameterTypes::COLOR)
//     {
//         int16_t index = -TBytes;
//         ((m_offsets[ParameterHelper::get(TTypes)] = index += TBytes), ...);
//     }
//
//     [[nodiscard]] uint16_t getOffset(ColorType type) const override
//     {
//         if (m_offsets.contains(ParameterHelper::get(type)))
//         {
//             return m_offsets.at(ParameterHelper::get(type));
//         }
//
//         throw std::out_of_range("Color does not exist");
//     }
// };
//
// int main()
// {
//     using ColorRGB = Color<1, ColorType::R, ColorType::G, ColorType::B>;
//     using ColorRGB16 = Color<2, ColorType::R, ColorType::G, ColorType::B>;
//     using ColorGB= Color<1, ColorType::G, ColorType::B>;
//
//     std::vector<std::shared_ptr<IfaceParameter>> vec; //
//     vec.push_back(std::make_shared<ColorRGB>());
//     vec.push_back(std::make_shared<ColorRGB16>());
//     vec.push_back(std::make_shared<ColorGB>());
//
//     for (auto& v : vec)
//     {
//     for (auto& [key, value] : v->getOffsets())
//     {
//         std::cout << key << " = " << value << std::endl;
//     }
//         std::cout << std::endl;
//     }
//     return 0;
// }





//
// Created by bobi on 10/09/2025.
//

#include "DMX/Fixture/Fixture.h"
#include <iostream>

#include "DMX/Universe.h"


int main()
{
    using namespace DMX;

    Universe uni(1);
    Fixture ledbar("Astera10");
    // ledbar.add<Parameters::PositionPan>();
    ledbar.addMultiple<Parameters::ColorRGB>(10);
    uni.add(ledbar);
    uni.add(ledbar);



    {
        auto fix = uni.getLight(0);
        auto colors = (*fix)[Parameters::ParameterTypes::COLOR];

        if (colors.has_value())
        {
            auto colorsV = colors.value().get();
            for (auto color : colorsV)
            {
                color->setValue("R", 80.f);
            }
        }
    }

    {
        auto fix = uni.getLight(1);
        auto colors = (*fix)[Parameters::ParameterTypes::COLOR];

        if (colors.has_value())
        {
            auto& colorsV = colors.value().get();
            for (auto& color : colorsV)
            {
                color->setValue("G", 70.f);
            }
        }
    }
    uni.print();
    // uni.getBytes();
    // try
    // {
    //
    // }
    // catch (const std::exception& e)
    // {
    //     std::cerr << e.what();
    // }
    // std::vector<std::shared_ptr<Parameter>> vec; //
    // vec.push_back(std::make_shared<ColorRGB>());
    // vec.push_back(std::make_shared<ColorRGB16>());
    // vec.push_back(std::make_shared<ColorW>());


    // for (auto& v : vec)
    // {
    // for (auto& [key, value] : v->getOffsets())
    // {
    //     std::cout << key << " = " << value << std::endl;
    // }
    //     std::cout << std::endl;
    // }
    return 0;
}
