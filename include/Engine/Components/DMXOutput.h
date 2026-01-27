//
// Created by bobi on 27. 01. 26.
//

#pragma once
#include <map>
#include <cstdint>
#include <string>
#include <set>

#include "DMX/Output/SacnSender.h"


namespace Components
{
    class DMXOutput
    {
        std::map<uint16_t, DMX::Output::SacnSender *> m_senders;
        std::string m_ip = "192.168.0.6";

    public:
        void check(uint16_t universe, uint8_t* bytes);
        void setIP(std::string ip);
        void update(const std::set<uint8_t>& dirtyUnis );
    };
}