#pragma once
#include <cstdint>
#include <list>
#include <optional>
#include <iostream>
#include <array>

// struct Fragmented
// {
//     uint32_t m_start = 0U;
//     uint32_t m_size = 0U;
//
//     // size_t m_size = 0U;
//     virtual ~Fragmented() = default;
//     [[nodiscard]] virtual uint32_t getSize() const = 0;
// };
// //
// template <typename T, size_t TSize>
// requires std::is_base_of_v<Fragmented, T>
// class FragmentedStorage
// {
//     struct Fragment
//     {
//         // protected:
//         std::shared_ptr<T> m_ptr = nullptr;
//         // uint8_t* m_buffer = nullptr;
//     };
//
//     std::list<Fragment> m_fragments;
//     uint8_t m_buffer[TSize] = {0U};
// public:
//     bool add(const T& fragment, std::optional<uint32_t> start = std::nullopt)
//     {
//         if (m_fragments.empty())
//         {
//
//             const uint32_t size = fragment.getSize();
//             const uint32_t startOffset = !start.has_value() ? 0 : start.value();
//             if (m_fragments.empty())
//             {
//                 if (size <= TSize)
//                 {
//                     fragment.setStart(startOffset);
//                     fragment.m_buffer = m_buffer;
//                     m_fragments.push_back(std::make_shared<T>(fragment));
//                     // fillBytesPatched(startOffset, startOffset + size);
//                 }
//                 else
//                 {
//                     std::cout << "Segment exceeds max size" << std::endl;
//                     return false;
//                 }
//             }
//
//         }
//     }
// };

 struct Fragment
 {
     // protected:
     uint32_t m_start = 0U;
     uint32_t m_size = 0U;
     uint8_t* m_buffer = nullptr;
     std::string m_name = "Name";

public:

    explicit Fragment(uint32_t size)
        : m_size(size)
    {}

    [[nodiscard]] uint32_t getStart() const { return m_start; }
    [[nodiscard]] uint32_t getSize() const { return m_size; }
    [[nodiscard]] uint32_t getEnd() const { return m_start + m_size - 1; }
    [[nodiscard]] uint32_t getOffset() const { return m_start + m_size; }
    [[nodiscard]] std::string getName() const { return m_name; }

    void setStart(uint32_t start) { m_start = start; }
    void setSize(uint32_t size) { m_size = size; }
    void setBuffer(uint8_t* buffer) { m_buffer = buffer; }
};

template <typename T, size_t TSize>
requires std::is_base_of_v<Fragment, T>
class FragmentedStorage
{
    std::list<T> m_fragments;
    std::array<uint8_t, TSize> m_buffer;
    std::array<bool, TSize> m_bytesPatched;

    void fillBytesPatched(uint16_t start, uint16_t end)
    {
        static uint16_t num = 1;
        for (uint16_t i = start; i < end; i++)
        {
            m_bytesPatched[i] = num;
        }

        num++;
    }
public:
    // bool add(const T& fragment, std::optional<uint32_t> start = std::nullopt)
    // {
    //     const uint32_t frag_size = m_fragments.size();
    //     if (m_fragments.empty())
    //     {
    //
    //     }
    // }

    void add(T& fragment, std::optional<uint32_t> start = std::nullopt)
    {
        if (m_fragments.empty())
        {
            if (fragment.getSize() <= TSize)
            {
                fragment.setStart(0);
                fragment.setBuffer(m_buffer.data());
                m_fragments.push_back(fragment);
                fillBytesPatched(0, fragment.getSize());
            }
            else
            {
                throw std::runtime_error("Segment is to big");
            }
        }
        else if (!start.has_value())
        {
            auto& f = m_fragments.back();

            if (f.getOffset() <= TSize)
            {
                fragment.setStart(f.getOffset());
                fragment.setBuffer(m_buffer.data());
                m_fragments.push_back(fragment);
                fillBytesPatched(fragment.getStart(), fragment.getOffset());
            }
            else
            {
                throw std::length_error("Segment size out of range");
            }
        }
        else
        {
            uint32_t _start = start.value();
            bool isFilled = m_bytesPatched[_start];
            for (uint32_t i = _start + 1; (i < _start + fragment.getSize()) && !isFilled; i++)
            {
                isFilled = m_bytesPatched[i];
            }

            if (!isFilled)
            {
                fragment.setStart(_start);
                fragment.setBuffer(m_buffer.data());
                m_fragments.push_back(fragment);
                fillBytesPatched(fragment.getStart(), fragment.getOffset());
            }
            else
            {
                throw std::runtime_error("Segment overlaps other segment");
            }
        }

        // m_fragmentsByName[fragment.getName()].push_back(getLight(m_fragments.size() - 1));

    }

    void addMultiple(const T& fragment, uint16_t amount, std::optional<uint32_t> start = std::nullopt)
    {
        for (int i = 0; i < amount; i++)
        {
            add(fragment, start);
        }
    }

    void printFragments() const
    {
        constexpr uint16_t printableOffset = 1;
        std::string col = "";

        for (int i = 0; i < m_fragments.size(); i++)
        {
            uint16_t offset = 0;

            auto it = m_fragments.begin();
            std::advance(it, i);
            auto &curr = *it;

            int16_t cstart = curr.getStart();
            int16_t cnext = curr.getOffset();
            int16_t cend = curr.getEnd();

            if (cstart != 0 && i == 0)
            {
                printf("\x1B[2m[%3d, %3d]\x1B[3m%s Unpatched %s\n", 0 + offset + printableOffset, cstart - 1 + offset + printableOffset, colorItalic.c_str(), colorReset.c_str());
            }

            // col = nextColor(cstart);

            // std::string temp =
            printf("%s[%3d, %3d] %s%s (%d) %s\n", col.c_str(), cstart + offset + printableOffset, cend + offset + printableOffset, colorItalic.c_str(), curr.getName().c_str(), curr.getSize(), colorReset.c_str());

            if (i < m_fragments.size() - 1)
            {
                std::advance(it, 1);
                auto &next = *it;
                if (curr.getOffset() != next.getStart())
                {
                    printf("\x1B[2m[%3d, %3d]\x1B[3m%s Unpatched %s\n", cnext + offset + printableOffset, next.getStart() - 1 + offset + printableOffset, colorItalic.c_str(), colorReset.c_str());
                    continue;
                }
            }
            else
            {
                if (curr.getOffset() < TSize)
                {
                    printf("\x1B[2m[%3d, %3d]\x1B[3m%s Unpatched %s\n", cnext + offset + printableOffset, TSize - 1 + offset + printableOffset, colorItalic.c_str(), colorReset.c_str());
                    continue;
                }
            }
        }
    }
};
