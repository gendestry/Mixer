// //
// // Created by bobi on 15/09/2025.
// //
//
#include "DMX/Universe.h"
#include <cmath>
#include <format>
#include <sstream>
#include <format>
#include <iostream>
#include "Helper/Utils.h"
//
namespace DMX {
//
//
     // std::string Universe::nextColor(int index) const
     // {
     //     std::vector<float> colorVecHsv = {110.0f, 0.5f, 0.8f};
     //     colorVecHsv[0] = fmod(colorVecHsv[0] + (bytesPatched[index] * 65), 360.0f);
     //     auto colorVec = Utils::hsvToRgb(colorVecHsv);
     //     return Utils::colorByRGB(colorVec[0], colorVec[1], colorVec[2], true);
     // }
//     std::shared_ptr<Fixture> Universe::getLight(std::size_t index)
//     {
//         auto it = m_fixtures.begin();
//         size_t current = 0;
//
//         while (it != m_fixtures.end())
//         {
//             if (current == index)
//             {
//                 return std::make_shared<Fixture>(*it);
//             }
//             ++it;
//             ++current;
//         }
//
//         return nullptr;
//     }
//
//     std::vector<std::shared_ptr<Fixture>> Universe::getLights(std::string name)
//     {
//         // std::vector<Light *> result;
//         // for (auto it = m_fixtures.begin(); it != lights.end(); ++it)
//         // {
//         //     if (it->m_Name == name)
//         //     {
//         //         result.push_back(&(*it));
//         //     }
//         // }
//         // return result;
//         return m_fixturesByName[name];
//     }
//
//     Fixture &Universe::operator[](int index)
//     {
//         if (index < 0 || index >= m_fixtures.size())
//         {
//             throw std::runtime_error("Index out of bounds");
//         }
//         auto it = m_fixtures.begin();
//         std::advance(it, index);
//         return *it;
//     }
//
//     std::vector<std::shared_ptr<Fixture>> Universe::operator[](Interval interval)
//     {
//         uint16_t start = interval.start;
//         uint16_t end = interval.end;
//         auto &all = m_fixturesByName[interval.name];
//
//         if (start >= all.size() || end >= all.size())
//         {
//             throw std::runtime_error("Index out of bounds");
//         }
//
//         std::vector<std::shared_ptr<Fixture>> result;
//         for (int i = start; i <= end; i++)
//         {
//             result.push_back(all[i]);
//         }
//
//         return result;
//     }
//
//     std::vector<uint8_t> Universe::getBytes() const
//      {
//          std::vector<uint8_t> bytes(MAX_SIZE, 0);
//          for (int i = 0; i < m_fixtures.size(); i++)
//          {
//              auto it = m_fixtures.begin();
//              std::advance(it, i);
//              const auto &curr = *it;
//              const uint8_t *currBytes = curr.getBytes();
//
//              for (int j = curr.start; j < curr.start + curr.getTotalSize(); j++)
//              {
//                  bytes[j] = currBytes[j - curr.start];
//              }
//          }
//
//          return bytes;
//      }
//
//
      void Universe::printFragments()
      {
          constexpr uint16_t printableOffset = 1;
          std::string col;

          for (int i = 0; i < storage.getSize(); i++)
          {
              const uint16_t offset = 0;

              // storage.getFragments()
              auto it = storage.getFragments().get().begin();
              std::advance(it, i);
              auto &curr = *it;

              uint16_t cstart = curr.getStart();
              uint16_t cnext = curr.getOffset();
              uint16_t cend = curr.getEnd();

              if (cstart != 0 && i == 0)
              {
                  // auto x = std::format("{}[{:3}, {:3}]", colorDim, (offset + printableOffset), (cstart - 1 + offset + printableOffset));
                  printf("\x1B[2m[%3d, %3d]\x1B[3m%s Unpatched %s\n", 0 + offset + printableOffset, cstart - 1 + offset + printableOffset, colorItalic.c_str(), colorReset.c_str());
              }

              // col = nextColor(cstart);
              col = "";
              printf("%s[%3d, %3d] %s%s (%d) %s\n", col.c_str(), cstart + offset + printableOffset, cend + offset + printableOffset, colorItalic.c_str(), curr.getName().c_str(), curr.getSize(), colorReset.c_str());

              if (i < storage.getSize() - 1)
              {
                  std::advance(it, 1);
                  auto &next = *it;
                  if (curr.getStart() + curr.getSize() != next.getStart())
                  {
                      printf("\x1B[2m[%3d, %3d]\x1B[3m%s Unpatched %s\n", cnext + offset + printableOffset, next.getStart() - 1 + offset + printableOffset, colorItalic.c_str(), colorReset.c_str());
                      continue;
                  }
              }
              else
              {
                  if (curr.getStart() + curr.getSize() < MAX_SIZE)
                  {
                      printf("\x1B[2m[%3d, %3d]\x1B[3m%s Unpatched %s\n", cnext + offset + printableOffset, MAX_SIZE - 1 + offset + printableOffset, colorItalic.c_str(), colorReset.c_str());
                      continue;
                  }
              }
          }
      }
//
//      void Universe::printBytes() const
//      {
//          std::vector<float> colorVecHsv = {110.0f, 0.5f, 0.8f};
//          std::vector<uint8_t> bytes = getBytes();
//
//          std::string col = nextColor(0);
//
//          int cond = std::min((int)bytes.size(), 16);
//          auto printSeperator = [&]()
//          {
//              for (int i = 0; i < cond; i++)
//              {
//                  std::cout << "----";
//                  if (i < cond - 1)
//                  {
//                      std::cout << "-";
//                  }
//              }
//              std::cout << std::endl;
//          };
//
//          std::cout << colorItalic;
//          for (int i = 0; i < cond; i++)
//          {
//              static const std::string hex = "0123456789ABCDEF";
//              std::cout << "0x" << hex[i] << "  ";
//          }
//          std::cout << colorReset << std::endl;
//
//          printSeperator();
//
//          for (int i = 0; i < bytes.size(); i++)
//          {
//              if (i % 16 == 0 && i != 0)
//                  std::cout << std::endl;
//
//              if (bytesPatched[i] != 0)
//                  col = nextColor(i);
//              else
//                  col = colorReset + "\x1B[2m" + "\x1B[3m";
//
//              std::cout << col << Utils::padByte(bytes[i], 3) << colorReset << "  ";
//          }
//          std::cout << std::endl;
//
//          printSeperator();
//      }
//
//      void Universe::print() const
//      {
//          std::cout << "Universe: " << m_universeID << ", Number of Lights: " << m_fixtures.size() << std::endl;
//          std::cout << "Patch list: " << std::endl;
//          printFragments();
//
//          std::cout << "Bytes: " << std::endl;
//          printBytes();
//      }
}