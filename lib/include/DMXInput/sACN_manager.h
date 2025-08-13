#pragma once

#include "DMXInput/sACN.h"
#include <unordered_map>
#include <memory>

namespace DMX::Input
{

class sACNManager
{
    std::unordered_map<uint8_t, std::shared_ptr<sACN>> m_universesMap;

    sACNManager() = default;

    // sACN getS
public:

    sACNManager(const sACNManager&) = delete;
    sACNManager& operator=(const sACNManager&) = delete;

    static sACNManager& get() {
        static sACNManager manager;
        return manager;
    }


    std::shared_ptr<sACN> getsACN(uint8_t universe)
    {
        if(!m_universesMap.contains(universe))
        {
            m_universesMap.insert({universe, std::make_shared<sACN>(universe)});
            // m_universesMap[universe]->connect(universe);
        }

        return m_universesMap[universe];
    }

    uint8_t* getDMXBuffer(uint8_t universe)
    {
        return getsACN(universe).get()->getDMXBuffer();
    }
};

};

