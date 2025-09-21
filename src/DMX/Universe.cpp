// //
// // Created by bobi on 15/09/2025.
// //
//
#include "DMX/Universe.h"
#include <format>
#include <sstream>
#include <iostream>
//
namespace DMX {
//
//

     std::shared_ptr<Fixture> Universe::getLight(std::size_t index)
     {
         auto it = m_fragments.begin();
         size_t current = 0;

         while (it != m_fragments.end())
         {
             if (current == index)
             {
                 return *it;
             }
             ++it;
             ++current;
         }

         return nullptr;
     }
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
//          for (int i = 0; i < m_fixtures.size(); i++)
//          {
//              auto it = m_fixtures.begin();
//              std::advance(it, i);
//              const auto &curr = *it;
//              const uint8_t *currBytes = curr->getBytes();
//
//              for (int j = curr->start; j < curr->start + curr.getTotalSize(); j++)
//              {
//                  bytes[j] = currBytes[j - curr.start];
//              }
//          }
//
//          return bytes;
//      }
//
//
//
      void Universe::print()
      {
          std::cout << "Universe: " << m_universeID << ", Number of Lights: " << getNumFragments() << std::endl;
          std::cout << "Patch list: " << std::endl;
          std::cout << fragmentsToString();

          std::cout << "Bytes: " << std::endl;
          std::cout << bytesToString();
      }
}