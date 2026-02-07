#include "Engine/Components/FixtureLibrary.h"

namespace Components
{

void FixtureLibrary::add(const DMX::Fixture& fix)
{
    m_fixtures[fix.name] = fix;
}

void FixtureLibrary::add(DMX::Fixture&& fixture)
{
    // m_fixtures[]
    // m_fixtures.insert(fixture.name, fixture);
    m_fixtures[fixture.name] = std::move(fixture);
}
void FixtureLibrary::readFromFile(const std::string& path)
{
    fileRead = true;
    std::ifstream file;
    file.open(path);

    std::string line;
    while (std::getline(file, line))
    {
        DMX::Fixture fixture;
        uint16_t i = 0U;
        bool isNumber = true;
        uint16_t number = 0;
        for (auto word : std::views::split(line, ','))
        {
            std::string w(word.begin(), word.end());

            if (i == 0U)
            {
                fixture.name = w;
            }
            else
            {
                if(isNumber)
                {
                    number = std::stoi(w);    
                }
                else
                {
                    for(uint16_t j = 0; j < number; j++)
                    {
                        if (!addFixtureParameter(fixture, w))
                        {
                            throw std::runtime_error("Failed to add fixture parameter: " + w);
                        }
                    }
                }
                isNumber = !isNumber;
                
            }
            i++;
        }

        // std::cout << fixture.name << fixture.isVirtualDIMRequired() << std::endl;

        if (fixture.isVirtualDIMRequired())
        {
            // fixture.add<DMX::Parameters::VDimmerParameter>();
            auto colors = fixture.getParameters(DMX::Parameters::Type::COLOR);
            if (colors.has_value())
            {
                // auto vdim = fixture.getParameters(DMX::Parameters::Type::VDIMMER).value();
                for (auto color : colors.value())
                {
                    fixture.add<DMX::Parameters::VDimmer>();
                }
            }
        }

        // if (vdimRequired)
        // {
            // fixture.add<DMX::Parameters::VDimmerParameter>();
            // std::cout << fixture.name<< std::endl;
            // fixture.add<DMX::Parameters::VDimmerParameter>();
            // auto colors = fixture.getParameters(DMX::Parameters::Type::COLOR);
            // if (colors.has_value())
            // {
                // auto vdim = fixture.getParameters(DMX::Parameters::Type::VDIMMER).value().back();

                // for (auto param : colors.value())
                // {
                    // DMX::Parameters::VDimmerParameter* raw = (DMX::Parameters::VDimmerParameter*)&vdim;
                    // raw->addColor(param);
                    // std::shared_ptr<DMX::Parameters::VDimmerParameter> casted = std::dynamic_pointer_cast<DMX::Parameters::VDimmerParameter>(param);
                    // param->a
                // }

            // }

        // }
        m_fixtures[fixture.name] = std::move(fixture);
    }
}
}
