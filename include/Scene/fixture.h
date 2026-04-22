//
// Created by bobi on 14. 02. 26.
//

#pragma once

#include <array>
#include <cstdint>

class Property {
};

enum class ColorType {
 R, G, B, W, A
};

template<ColorType... TTypes>
struct ColorParameter final : public Property {
 static constexpr std::size_t k_size = sizeof...(TTypes) * TBytes;
 std::array<float, k_size> m_data;

 ColorParameter()
  : Property() {
 }
};

class Fixture {
public:
};
