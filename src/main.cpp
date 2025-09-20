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

// #include "DMX/Universe.h"
#include "DMX/Universe.h"
#include "Helper/FragmentedStorage.h"

struct Data : public Fragment
{
    Data(uint32_t size)
        : Fragment(size)
    {}
};

int main()
{
    using namespace DMX;

    Fixture ledbar("led");
    ledbar.addMultiple<Parameters::ColorRGB>(10);

    Universe uni(1);
    uni.add(ledbar);
    uni.add(ledbar);
    uni.printFragments();
    // FragmentedStorage<Data, 100> storage;
    // Data d(5);
    // try
    // {
    //     storage.add(d);
    //     storage.add(d, 5);
    //     storage.add(d, 8);
    // }
    // catch (const std::exception& e)
    // {
    //     std::cout << e.what() << std::endl;
    //     // return 1;
    // }
    // storage.add(d, 15);
    // storage.printFragments();

    // Universe uni(1);
    // Fixture par("Parica");
    // par.add<Parameters::Dimmer>();
    // par.add<Parameters::ColorRGB>();
    // //
    // // std::cout << par.describe();
    //
    // Fixture ledbar("Astera10");
    // ledbar.addMultiple<Parameters::ColorRGB>(5);
    //
    // // uni.addMultiple(par, 5);
    // uni.add(ledbar);
    // uni.add(par);
    //
    // std::cout << par.describe();
    // std::cout << ledbar.describe();
    //
    //
    // {
    //     auto fix = uni["Astera10"];
    //     // auto lbfix = uni["Astera10"];
    //     // fix.insert(fix.end(), lbfix.begin(), lbfix.end());
    //
    //     for (auto &f : fix)
    //     {
    //
    //         auto colors = (*f)[Parameters::ParameterTypes::COLOR];
    //         if (colors.has_value())
    //         {
    //             // std::cout << colors.value().get().size() << std::endl;
    //             auto colorsV = colors.value().get();
    //             // colorsV[0]->setValue("R", 10.f);
    //             // colorsV[1]->setValue("R", 20.f);
    //             // colorsV[2]->setValue("R", 30.f);
    //             // colorsV[3]->setValue("R", 40.f);
    //             // colorsV[4]->setValue("R", 50.f);
    //             // for (auto color : colorsV)
    //             // {
    //             //     color->setValue("R", 80.f);
    //             // }
    //         }
    //     }
    //
    // }
    // //
    // // {
    // //     auto fix = uni.getLight(1);
    // //     auto colors = (*fix)[Parameters::ParameterTypes::COLOR];
    // //
    // //     if (colors.has_value())
    // //     {
    // //         auto& colorsV = colors.value().get();
    // //         for (auto& color : colorsV)
    // //         {
    // //             color->setValue("G", 70.f);
    // //         }
    // //     }
    // // }
    // uni.print();
    // // uni.getBytes();
    // // try
    // // {
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
