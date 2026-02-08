//
// Created by bobi on 06/10/2025.
//

#pragma once
#include <cstdint>
#include <vector>
#include <variant>

namespace Utils
{

class Curve
{
protected:
    std::vector<float> m_values;
    uint32_t m_length = 1;
    uint16_t m_peaks = 1;

    float m_min = 0.f;
    float m_max = 1.f;

    void update();
    void init();
    virtual void fill() {};
public:
    enum Type
    {
        SINUSOID,
        TRIANGLE,
        SQUARE,
        LINE
    } m_type;

    Curve() = delete;//default;
    explicit Curve(Type type, uint16_t length, uint16_t peaks = 1U);
    explicit Curve(Type type, float min, float max, uint16_t length, uint16_t peaks = 1U);

    virtual ~Curve() = default;

    void setType(const Type type) { m_type = type; update(); }
    void setLength(const uint32_t length) { m_length = length; init();}
    void setPeaks(const uint16_t peaks) { m_peaks = peaks; update();}

    uint32_t getLength() const { return m_length; }
    // void init(Type type, uint16_t length, uint16_t peaks = 1U);

    float operator[] (std::size_t index) const;
};

class Sinusoid : public Curve
{
protected:
    void fill() override;
public:
    Sinusoid(uint16_t length, uint16_t peaks = 1U)
        : Curve(SINUSOID, length, peaks) {
        init();
    }

    Sinusoid(float min, float max, uint16_t length, uint16_t peaks = 1U)
        : Curve(SINUSOID, min, max, length, peaks) {
        init();
    }
};

class Triangle : public Curve
{
protected:
    void fill() override;
public:
    Triangle(uint16_t length, uint16_t peaks = 1U)
        : Curve(TRIANGLE, length, peaks) {
        init();
    }

    Triangle(uint16_t length, float min, float max, uint16_t peaks = 1U)
        : Curve(TRIANGLE, min, max, length, peaks) {
        init();
    }
};

class Square : public Curve
{
protected:
    void fill() override;
public:
    Square(uint16_t length, uint16_t peaks = 1U)
        : Curve(SQUARE, length, peaks) {
        init();
    }

    Square(uint16_t length, float min, float max, uint16_t peaks = 1U)
        : Curve(SQUARE, min, max, length, peaks) {
        init();
    }
};

    using CurveVariant = std::variant<Utils::Sinusoid, Utils::Triangle, Utils::Square>;
}
