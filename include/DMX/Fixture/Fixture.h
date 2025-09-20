//
// Created by bobi on 15/09/2025.
//

#pragma once
#include <vector>
#include <memory>
#include <type_traits>
#include <optional>

#include "Helper/FragmentedStorage.h"
#include "Parameters/Parameters.h"

const std::string colorGreen = "\x1B[32m";
const std::string colorYellow = "\x1B[33m";
const std::string colorBlue = "\x1B[34m";
const std::string colorReset = "\x1B[0m";
const std::string colorDim = "\x1B[2m";
const std::string colorItalic = "\x1B[3m";


namespace DMX
{

class Fixture : public Fragment
{
    std::vector<std::shared_ptr<Parameters::Parameter>> m_Parameters;
    std::unordered_map<Parameters::ParameterTypes, std::vector<std::shared_ptr<Parameters::Parameter>>> m_indexes;
    // std::size_t m_totalSize = 0;

    uint16_t m_FID = 0U;
    std::string m_name;
public:
    // uint16_t start = 0;
    uint8_t* m_bytes = nullptr;

    explicit Fixture(const std::string& name);
    Fixture(const Fixture& other);

    template<typename T>
    requires std::is_base_of_v<Parameters::Parameter, T>
    void add()
    {
        m_Parameters.emplace_back(std::make_shared<T>(m_bytes, m_size));
        m_indexes[m_Parameters.back()->getType()].push_back(m_Parameters.back());
        m_size += m_Parameters.back()->getSize();
    }

    template<typename T>
    requires std::is_base_of_v<Parameters::Parameter, T>
    void addMultiple(uint16_t amount)
    {
        for (uint16_t i = 0; i < amount; ++i)
        {
            add<T>();
        }
    }

    [[nodiscard]] std::optional<std::reference_wrapper<std::vector<std::shared_ptr<Parameters::Parameter>>>> getParameters(Parameters::ParameterTypes type);
    [[nodiscard]] std::optional<std::reference_wrapper<std::vector<std::shared_ptr<Parameters::Parameter>>>> operator[](Parameters::ParameterTypes type);

    // [[nodiscard]] std::string getName() const;
    // [[nodiscard]] std::size_t getTotalSize() const;
    [[nodiscard]] uint8_t* getBytes() const;

    [[nodiscard]] std::string describe() const;
};

}
