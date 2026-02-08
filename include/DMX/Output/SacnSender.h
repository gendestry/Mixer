#pragma once
#include <arpa/inet.h>
#include <array>
#include <string>

namespace DMX::Output
{

class SacnSender
{
    uint8_t m_universe;
    uint8_t m_sequence;

    bool m_inited = false;
    int m_socket;
    sockaddr_in m_destinationIp{};
    std::array<uint8_t, 638> m_packet;
    uint8_t *m_buffer;
    // uint8_t m_packet[638];

    void initializePacket();

public:
    SacnSender();
    SacnSender(uint8_t universe, const char *localIp);
    SacnSender(SacnSender &&other);

    ~SacnSender();

    void setIP(const char *ip);
    void setBuffer(uint8_t *buffer);
    void begin(uint8_t universe, const char *localIp);
    void send();

    void setSourceName(const std::string& name);

};
}
