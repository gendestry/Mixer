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
#include "Traits/Serialize.h"

namespace DMX
{

class Fixture : public Utils::Fragment, Traits::Serializable
{
    std::list<std::shared_ptr<Parameters::Parameter>> m_Parameters;
    std::unordered_map<Parameters::Type, std::list<std::shared_ptr<Parameters::Parameter>>> m_indexes;
    bool vDimRequired = true;
    
public:
    uint8_t m_universe;
    
    Fixture() = default;
    explicit Fixture(const std::string& name);
    Fixture(const Fixture& other);

    template<typename T>
    requires std::is_base_of_v<Parameters::Parameter, T>
    void add()
    {
        m_Parameters.emplace_back(std::make_shared<T>(buffer, start + size));
        m_indexes[m_Parameters.back()->getType()].push_back(m_Parameters.back());
        size += m_Parameters.back()->getSize();

        if (m_Parameters.back()->getType() == Parameters::Type::DIMMER)
        {
            vDimRequired = false;
        }
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

    [[nodiscard]] bool isVirtualDIMRequired() const
    {
        return vDimRequired;
    }

    void setStart(uint32_t start) override;
    void setBuffer(uint8_t* buffer) override;
    void setName(const std::string& newName) {name = newName;}

    void print() {
        for (auto param : m_Parameters) {
            printf("%p\n", param->getBuffer());
        }
    }

    template<Parameters::Type TType>
    [[nodiscard]] size_t paramsOfType() {
        if (m_indexes.contains(TType)) {
            return m_indexes[TType].size();
        }

        return 0;
    }

    void setIntensity(float percentage)
    {
        if (!m_indexes.contains(Parameters::Type::DIMMER))
        {
            auto colors = m_indexes[Parameters::Type::COLOR];
            if (colors.size() == 0)
            {
                return;
            }

            for (auto color : colors)
            {
                color->multiplyValue("R", percentage);
                color->multiplyValue("G", percentage);
                color->multiplyValue("B", percentage);
                color->multiplyValue("W", percentage);
                color->multiplyValue("A", percentage);
            }
        }
    }

    [[nodiscard]] std::optional<std::list<std::shared_ptr<Parameters::Parameter>>> getParameters(Parameters::Type type);
    [[nodiscard]] std::optional<std::list<std::shared_ptr<Parameters::Parameter>>> operator[](Parameters::Type type);

    // [[nodiscard]] std::string getName() const;
    // [[nodiscard]] std::size_t getTotalSize() const;
    [[nodiscard]] uint8_t* getBytes() const;

    std::string toString() const override;

    // [[nodiscard]] std::string describe() const;
};

}
