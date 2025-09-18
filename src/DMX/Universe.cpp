//
// Created by bobi on 15/09/2025.
//

#include "DMX/Universe.h"
#include <cmath>
#include <iostream>
#include "Helper/Utils.h"

namespace DMX {


    std::string Universe::nextColor(int index) const
    {
        std::vector<float> colorVecHsv = {110.0f, 0.5f, 0.8f};
        colorVecHsv[0] = fmod(colorVecHsv[0] + (bytesPatched[index] * 65), 360.0f);
        auto colorVec = Utils::hsvToRgb(colorVecHsv);
        return Utils::colorByRGB(colorVec[0], colorVec[1], colorVec[2], true);
    }

    void Universe::fillBytesPatched(uint16_t start, uint16_t end)
    {
        static uint16_t num = 1;
        for (uint16_t i = start; i < end; i++)
        {
            bytesPatched[i] = num;
        }

        num++;
    }

    bool Universe::add(Fixture &fragment, std::optional<uint16_t> start)
    {
        if (m_fixtures.empty())
        {
            uint16_t size = fragment.getTotalSize();
            if (size <= MAX_SIZE)
            {
                uint16_t startOffset = !start.has_value() ? 0 : start.value();
                fragment.start = startOffset;
                fragment.m_bytes = m_bytes;
                // fragment.m_ID = Light::incrementID(fragment.m_Name);

                m_fixtures.push_back(fragment);
                // std::cout << startOffset << std::endl;
                fillBytesPatched(startOffset, startOffset + size);
            }
            else
            {
                std::cout << "Segment exceeds max size" << std::endl;
                return false;
            }
        }
        else
            // insert next
            if (!start.has_value())
            {
                auto &frag = m_fixtures.back();
                uint16_t _start = frag.start;
                // if segment fits at end
                if (_start + frag.getTotalSize() + fragment.getTotalSize() <= MAX_SIZE)
                {
                    fragment.start = _start + frag.getTotalSize();
                    // std::cout << fragment.start << std::endl;
                    fragment.m_bytes = m_bytes;
                    // fragment.m_ID = Light::incrementID(fragment.m_Name);
                    m_fixtures.push_back(fragment);
                    fillBytesPatched(fragment.start, fragment.start + fragment.getTotalSize());
                }
            }
            else
            {
                std::size_t size = fragment.getTotalSize();
                for (int i = 0; i < m_fixtures.size(); i++)
                {
                    auto it = m_fixtures.begin();
                    std::advance(it, i);
                    auto current = *it;

                    // not last segment
                    if (i < m_fixtures.size() - 1)
                    {
                        std::advance(it, 1);
                        auto &next = *it;
                        if (start.value() >= current.start && start.value() < current.start + current.getTotalSize())
                        {
                            std::cout << "Segment within bounds" << std::endl;
                            return false;
                        }
                        // same for next
                        if (start.value() >= next.start && start.value() < next.start + next.getTotalSize())
                        {
                            std::cout << "Segment within bounds" << std::endl;
                            return false;
                        }
                        if (current.start + current.getTotalSize() <= start.value() && start.value() + size <= next.start)
                        {
                            fragment.start = start.value();
                            fragment.m_bytes = m_bytes;
                            // fragment.m_ID = Light::incrementID(fragment.m_Name);

                            m_fixtures.insert(it, fragment);
                            fillBytesPatched(start.value(), start.value() + size);
                        }
                        else
                        {
                            continue;
                        }
                    }
                    else
                    {
                        // check if segment within current segment bounds
                        if (start.value() >= current.start && start.value() < current.start + current.getTotalSize())
                        {
                            std::cout << "Segment within bounds 1" << std::endl;
                            return false;
                        }

                        if (start.value() >= MAX_SIZE)
                        {
                            std::cout << "Segment start.value is out of bounds 2" << std::endl;
                            return false;
                        }

                        if (start.value() + size <= MAX_SIZE)
                        {
                            fragment.start = start.value();
                            fragment.m_bytes = m_bytes;
                            // fragment.m_ID = Light::incrementID(fragment.m_Name);

                            m_fixtures.push_back(fragment);
                            fillBytesPatched(start.value(), start.value() + size);
                        }
                        else
                        {
                            std::cout << "Segment exceeds max size" << std::endl;
                            return false;
                        }
                    }
                }
            }
        m_fixturesByName[fragment.getName()].push_back(getLight(m_fixtures.size() - 1));
        return true;
    }

    bool Universe::addMultiple(Fixture &fragment, uint16_t amount, std::optional<uint16_t> start)
    {
        for (int i = 0; i < amount; i++)
        {
            if (!add(fragment, start))
            {
                return false;
            }
        }
        return true;
    }

    std::shared_ptr<Fixture> Universe::getLight(std::size_t index)
    {
        auto it = m_fixtures.begin();
        size_t current = 0;

        while (it != m_fixtures.end())
        {
            if (current == index)
            {
                return std::make_shared<Fixture>(*it);
            }
            ++it;
            ++current;
        }

        return nullptr;
    }

    std::vector<std::shared_ptr<Fixture>> Universe::getLights(std::string name)
    {
        // std::vector<Light *> result;
        // for (auto it = m_fixtures.begin(); it != lights.end(); ++it)
        // {
        //     if (it->m_Name == name)
        //     {
        //         result.push_back(&(*it));
        //     }
        // }
        // return result;
        return m_fixturesByName[name];
    }

    Fixture &Universe::operator[](int index)
    {
        if (index < 0 || index >= m_fixtures.size())
        {
            throw std::runtime_error("Index out of bounds");
        }
        auto it = m_fixtures.begin();
        std::advance(it, index);
        return *it;
    }

    std::vector<std::shared_ptr<Fixture>> Universe::operator[](Interval interval)
    {
        uint16_t start = interval.start;
        uint16_t end = interval.end;
        auto &all = m_fixturesByName[interval.name];

        if (start >= all.size() || end >= all.size())
        {
            throw std::runtime_error("Index out of bounds");
        }

        std::vector<std::shared_ptr<Fixture>> result;
        for (int i = start; i <= end; i++)
        {
            result.push_back(all[i]);
        }

        return result;
    }

    std::vector<uint8_t> Universe::getBytes() const
     {
         std::vector<uint8_t> bytes(MAX_SIZE, 0);
         for (int i = 0; i < m_fixtures.size(); i++)
         {
             auto it = m_fixtures.begin();
             std::advance(it, i);
             const auto &curr = *it;
             const uint8_t *currBytes = curr.getBytes();

             for (int j = curr.start; j < curr.start + curr.getTotalSize(); j++)
             {
                 bytes[j] = currBytes[j - curr.start];
             }
         }

         return bytes;
     }


     void Universe::printFragments() const
     {
         constexpr uint16_t printableOffset = 1;
         std::string col;

         for (int i = 0; i < m_fixtures.size(); i++)
         {
             const uint16_t offset = 0;

             auto it = m_fixtures.begin();
             std::advance(it, i);
             auto &curr = *it;

             int16_t cstart = curr.start;
             int16_t cnext = curr.start + curr.getTotalSize();
             int16_t cend = curr.start + curr.getTotalSize() - 1;

             if (cstart != 0 && i == 0)
             {
                 printf("\x1B[2m[%3d, %3d]\x1B[3m%s Unpatched %s\n", 0 + offset + printableOffset, cstart - 1 + offset + printableOffset, colorItalic.c_str(), colorReset.c_str());
             }

             col = nextColor(cstart);

             printf("%s[%3d, %3d] %s%s (%d) %s\n", col.c_str(), cstart + offset + printableOffset, cend + offset + printableOffset, colorItalic.c_str(), curr.getName().c_str(), curr.getTotalSize(), colorReset.c_str());

             if (i < m_fixtures.size() - 1)
             {
                 std::advance(it, 1);
                 auto &next = *it;
                 if (curr.start + curr.getTotalSize() != next.start)
                 {
                     printf("\x1B[2m[%3d, %3d]\x1B[3m%s Unpatched %s\n", cnext + offset + printableOffset, next.start - 1 + offset + printableOffset, colorItalic.c_str(), colorReset.c_str());
                     continue;
                 }
             }
             else
             {
                 if (curr.start + curr.getTotalSize() < MAX_SIZE)
                 {
                     printf("\x1B[2m[%3d, %3d]\x1B[3m%s Unpatched %s\n", cnext + offset + printableOffset, MAX_SIZE - 1 + offset + printableOffset, colorItalic.c_str(), colorReset.c_str());
                     continue;
                 }
             }
         }
     }

     void Universe::printBytes() const
     {
         std::vector<float> colorVecHsv = {110.0f, 0.5f, 0.8f};
         std::vector<uint8_t> bytes = getBytes();

         std::string col = nextColor(0);

         int cond = std::min((int)bytes.size(), 16);
         auto printSeperator = [&]()
         {
             for (int i = 0; i < cond; i++)
             {
                 std::cout << "----";
                 if (i < cond - 1)
                 {
                     std::cout << "-";
                 }
             }
             std::cout << std::endl;
         };

         std::cout << colorItalic;
         for (int i = 0; i < cond; i++)
         {
             static const std::string hex = "0123456789ABCDEF";
             std::cout << "0x" << hex[i] << "  ";
         }
         std::cout << colorReset << std::endl;

         printSeperator();

         for (int i = 0; i < bytes.size(); i++)
         {
             if (i % 16 == 0 && i != 0)
                 std::cout << std::endl;

             if (bytesPatched[i] != 0)
                 col = nextColor(i);
             else
                 col = colorReset + "\x1B[2m" + "\x1B[3m";

             std::cout << col << Utils::padByte(bytes[i], 3) << colorReset << "  ";
         }
         std::cout << std::endl;

         printSeperator();
     }

     void Universe::print() const
     {
         std::cout << "Universe: " << m_universeID << ", Number of Lights: " << m_fixtures.size() << std::endl;
         std::cout << "Patch list: " << std::endl;
         printFragments();

         std::cout << "Bytes: " << std::endl;
         printBytes();
     }
}