//
// Created by Bobi on 9/21/25.
//

#pragma once
#include <sstream>
#include <cstdint>
#include <array>
#include <cmath>
#include <format>
#include <list>
#include <memory>
#include <optional>
#include <type_traits>
#include <vector>

#include "Printable.h"
#include "Utils.h"

namespace Utils
{
const std::string colorGreen = "\x1B[32m";
const std::string colorYellow = "\x1B[33m";
const std::string colorBlue = "\x1B[34m";
const std::string colorReset = "\x1B[0m";
const std::string colorDim = "\x1B[2m";
const std::string colorItalic = "\x1B[3m";

struct Fragment : public Traits::Printable
{
    uint32_t start = 0U;
    uint32_t size = 0U;
    uint8_t* buffer = nullptr;
    uint16_t id = 0U;
    std::string name;

    Fragment() = default;
    virtual ~Fragment() = default;
    explicit Fragment(const std::string& name) : name(name) {}
    explicit Fragment(uint32_t size) : size(size) {}
    explicit Fragment(uint32_t start, uint32_t size) : start(start), size(size) {}

    virtual void setStart(uint32_t st) { start = st; };
    virtual void setBuffer(uint8_t* buf) {buffer = buf;}

    [[nodiscard]] std::string describe() const override
    {
        std::stringstream ss;
        ss << "[" << id << "] \"" << name << "\": " << size << " bytes";
        return ss.str();
        // return std::format(R"({})", name);
    }

};

template <typename T, size_t TSize>
requires std::is_base_of_v<Fragment, T>
class FragmentedStorage
{
protected:
    std::list<std::shared_ptr<T>> m_fragments;
    std::array<uint8_t, TSize> m_buffer = { 0U };
    std::array<uint16_t, TSize> m_bytesPatched = { 0U };
    uint16_t m_fragmentsNum = 1U;

    void fillBytesPatched(uint32_t start, uint32_t end)
    {
        std::fill(m_bytesPatched.begin() + start, m_bytesPatched.begin() + end, m_fragmentsNum);
        m_fragmentsNum++;
    }

    [[nodiscard]] std::string nextColor(int index) const
    {
        std::vector<float> colorVecHsv = {110.0f, 0.5f, 0.8f};
        colorVecHsv[0] = std::fmod(colorVecHsv[0] + (m_bytesPatched[index] * 65), 360.0f);
        const auto colorVec = hsvToRgb(colorVecHsv);
        return colorByRGB(colorVec[0], colorVec[1], colorVec[2], true);
    }

    void add(std::shared_ptr<T> newFragment, const std::optional<uint32_t> start = std::nullopt) {
        // std::shared_ptr<T> newFragment = std::make_shared<T>(fragment);
        // T& fragment = newFragment.get();
        newFragment->setBuffer(m_buffer.data());

        if (!start.has_value())
        {
            const uint32_t offset = m_fragments.empty() ? 0 : m_fragments.back()->start + m_fragments.back()->size;
            if (offset > TSize)
            {
                throw std::out_of_range("Out of range");
            }

            newFragment->setStart(offset);
            fillBytesPatched(offset, offset + newFragment->size);
            m_fragments.emplace_back(std::move(newFragment));
        }
        else
        {
            uint32_t _start = start.value();
            bool isFilled = m_bytesPatched[_start];
            for (uint32_t i = _start + 1; (i < _start + newFragment->size) && !isFilled; i++)
            {
                isFilled = m_bytesPatched[i];
            }

            if (isFilled)
            {
                throw std::runtime_error("Fragment overlaps other segment");
            }

            newFragment->setStart(_start);
            fillBytesPatched(_start, newFragment->start + newFragment->size);
            m_fragments.push_back(std::move(newFragment));
        }
    }
public:

    [[nodiscard]] size_t getNumFragments() const
    {
        return m_fragments.size();
    }

    [[nodiscard]] std::string fragmentsToString() const
    {
        std::stringstream ss;
        std::string col = nextColor(0);
        uint32_t curr = 0U;
        for (const auto& fragment : m_fragments)
        {
            const uint32_t& start = fragment->start;
            col = nextColor(start);
            if (curr != start)
            {
                ss << std::format("{}[{:3}, {:3}]{} Unpatched{}\n", colorDim, 1, start, colorItalic, colorReset);
            }
            ss << std::format("{}[{:3}, {:3}]{} \"{}\" FID: {} ({} bytes){}\n", col, start + 1, start + fragment->size, colorItalic, fragment->name, fragment->id, fragment->size, colorReset);
            curr = start + fragment->size;
        }

        if (curr != TSize - 1)
        {
            ss << std::format("{}[{:3}, {:3}]{} Unpatched{}\n", colorDim, curr + 1, TSize, colorItalic, colorReset);
        }
        return ss.str();
    }

    [[nodiscard]] std::string bytesToString() const
    {
        std::stringstream ss;
        std::vector<float> colorVecHsv = {110.0f, 0.5f, 0.8f};
        const auto& bytes = m_buffer;

        std::string col = nextColor(0);

        const int32_t cond = std::min(static_cast<int32_t>(TSize), 16);
        auto printSeperator = [&]()
        {
            for (int i = 0; i < cond; i++)
            {
                ss << "----";
                if (i < cond - 1)
                {
                    ss << "-";
                }
            }
            ss << std::endl;
        };

        ss << colorItalic;
        for (int i = 0; i < cond; i++)
        {
            static const std::string hex = "0123456789ABCDEF";
            ss << "0x" << hex[i] << "  ";
        }
        ss << colorReset << std::endl;

        printSeperator();

        for (int i = 0; i < bytes.size(); i++)
        {
            if (i % 16 == 0 && i != 0)
            {
                ss << std::endl;
            }

            if (m_bytesPatched[i] != 0)
            {
                col = nextColor(i);
            }
            else
            {
                col = colorReset + colorDim + colorReset;
            }

            ss << col << Utils::padByte(bytes[i], 3) << colorReset << "  ";
        }
        ss << std::endl;

        printSeperator();

        return ss.str();
    }
};

}

