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


    // explicit Fragment(uint32_t size)
    //     : m_size(size)
    // {}

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
class old_FragmentedStorage {
protected:
    std::list<std::unique_ptr<T>> m_fragments;
    std::array<uint8_t, TSize> m_buffer = {0};
    std::array<bool, TSize> m_bytesPatched = {false};

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

    void add(T fragment, std::optional<uint32_t> start = std::nullopt)
    {
        std::unique_ptr<T> newFragment = std::make_unique<T>(fragment);
        if (m_fragments.empty())
        {
            if (fragment.getSize() <= TSize)
            {

                newFragment->setStart(0);
                // newFragment->set
                newFragment->setBuffer(m_buffer.data());
                fillBytesPatched(0, newFragment->getSize());
                m_fragments.push_back(std::move(newFragment));
            }
            else
            {
                throw std::runtime_error("Segment is to big");
            }
        }
        else if (!start.has_value())
        {
            auto& f = m_fragments.back();

            if (f->getOffset() <= TSize)
            {
                newFragment->setStart(f->getOffset());
                newFragment->setBuffer(m_buffer.data());
                fillBytesPatched(newFragment->getStart(), newFragment->getOffset());
                m_fragments.emplace_back(std::move(newFragment));
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
                newFragment->setStart(_start);
                newFragment->setBuffer(m_buffer.data());
                fillBytesPatched(newFragment->getStart(), newFragment->getOffset());
                m_fragments.emplace_back(std::move(newFragment));
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

    // [[nodiscard]] std::reference_wrapper<std::list<T>> getFragments()
    // {
    //     return m_fragments;
    // }

    [[nodiscard]] uint16_t getSize() const { return m_fragments.size(); }
    [[nodiscard]] const std::array<uint8_t, TSize>& getBuffer() const { return m_buffer; }
    [[nodiscard]] const std::array<bool, TSize>& getBytesPatched() const { return m_bytesPatched; }

    [[nodiscard]] std::reference_wrapper<std::list<std::unique_ptr<T>>> getFragments() { return m_fragments; }
};
