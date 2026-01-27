//
// Created by bobi on 27. 01. 26.
//

#include "Engine/Components/DMXOutput.h"

namespace Components
{
    void DMXOutput::check(uint16_t universe, uint8_t* bytes)
    {
        if (!m_senders.contains(universe))
        {
            DMX::Output::SacnSender *sender = new DMX::Output::SacnSender(universe, m_ip.c_str());
            sender->setBuffer(bytes);
            m_senders[universe] = sender;
        }
    }

    void DMXOutput::setIP(std::string ip)
    {
        m_ip = ip;

        for (auto &[key, value] : m_senders)
        {
            value->setIP(ip.c_str());
        }
    }

    void DMXOutput::update(const std::set<uint8_t>& dirtyUnis )
    {
        for (auto &d : dirtyUnis)
        {
            m_senders[d]->send();
        }
    }
}