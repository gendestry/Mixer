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

namespace DMX
{

class Fixture : public Utils::Fragment
{
    std::list<std::shared_ptr<Parameters::Parameter>> m_Parameters;
    std::unordered_map<Parameters::ParameterTypes, std::list<std::shared_ptr<Parameters::Parameter>>> m_indexes;

    uint16_t m_FID = 0U;
public:

    explicit Fixture(const std::string& name);
    Fixture(const Fixture& other);

    template<typename T>
    requires std::is_base_of_v<Parameters::Parameter, T>
    void add()
    {
        m_Parameters.emplace_back(std::make_shared<T>(buffer, start));
        m_indexes[m_Parameters.back()->getType()].push_back(m_Parameters.back());
        size += m_Parameters.back()->getSize();
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

    void setStart(uint32_t start) override;
    void setBuffer(uint8_t* buffer) override;

    void print() {
        for (auto param : m_Parameters) {
            printf("%p\n", param->getBuffer());
        }
    }

    template<Parameters::ParameterTypes TType>
    [[nodiscard]] size_t paramsOfType() {
        if (m_indexes.contains(TType)) {
            return m_indexes[TType].size();
        }

        return 0;
    }

    [[nodiscard]] std::optional<std::list<std::shared_ptr<Parameters::Parameter>>> getParameters(Parameters::ParameterTypes type);
    [[nodiscard]] std::optional<std::list<std::shared_ptr<Parameters::Parameter>>> operator[](Parameters::ParameterTypes type);

    // [[nodiscard]] std::string getName() const;
    // [[nodiscard]] std::size_t getTotalSize() const;
    [[nodiscard]] uint8_t* getBytes() const;

    [[nodiscard]] std::string describe() const;
};

}
