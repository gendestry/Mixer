//
// Created by bobi on 7. 02. 26.
//

#pragma once
#include <string>
#include <unordered_map>

struct NetworkUtils {
    static std::unordered_map<std::string, std::string> m_ips;

    static void scan();
    static void rescan();

    static std::unordered_map<std::string, std::string> IPs();

    static std::string primaryIP();
};