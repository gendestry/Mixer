#pragma once

#include "Fixture/Fixture.h"
#include "Fixture/Fixture1D.h"
#include "Fixture/Fixture2D.h"

inline auto MakeAstera60()
{
    return Fixture::make_fixture<Fixture::Fixture1D, DMX::Output::LedOrder::RGB, 5>(
        std::vector<DMX::Preset>
        {
            {{1}, "group by 1"},
            {{2}, "group by 2"},
            {{4}, "group by 4"},
            {{10}, "group by 10"}
        },
        DMX::DMXInfo{1U, 0U, 1U},
        60);
};

inline auto MakeStrobo24x8()
{
    return Fixture::make_fixture<Fixture::Fixture2D, DMX::Output::LedOrder::RGB, 5>(
       std::vector<DMX::Preset>
       {
           {{1, 1}, "1x1 grid"},
           {{4, 1}, "4x1 grid"},
           {{4, 2}, "4x2 grid"},
           {{12, 4}, "12x4 grid"}
       },
       DMX::DMXInfo{1U, 0U, 0U},
       24, 8);
}