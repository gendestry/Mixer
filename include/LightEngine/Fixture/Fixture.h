#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Utils/Colors/RGB.h"
#include "Utils/Storage/FragmentedStorage.h"

#include "LightEngine/Fixture/Parameters/Attributes.h"

//
// Fixture: one patched lighting fixture. It is a Utils::Fragment, so it lives
// inside a Universe (Utils::FragmentedStorage) which assigns it a start address
// and owns the DMX buffer. A fixture is a stack of Parameters laid out
// contiguously; its footprint (Fragment::size) is the sum of their sizes.
//
// Placement flow: the storage copies the fixture and calls setStart(); the
// owning Universe then calls setBuffer() with its buffer. Both are propagated
// down to every parameter, so each parameter ends up pointing at the right
// slice of the universe buffer (absolute offset = fixture start + local offset).
//
namespace LightEngine
{
    class Fixture : public Utils::Fragment
    {
        using ParamPtr = std::shared_ptr<Parameters::Parameter>;

        std::string                                       m_name = "fixture";
        uint16_t                                          m_fid = 0;      // global fixture id (assigned by Patch)
        uint16_t                                          m_universe = 0; // universe it is patched into
        std::vector<ParamPtr>                             m_parameters;  // in layout order
        std::map<Parameters::Type, std::vector<ParamPtr>> m_byType;      // indexed by capability
        uint8_t*                                          buffer = nullptr; // -> universe buffer (non-owning)

        [[nodiscard]] Parameters::Parameter* firstOfType(Parameters::Type t) const;
        void rebindVirtualDimmers();
        void cloneParametersFrom(const Fixture& other);

    public:
        Fixture() = default;
        explicit Fixture(std::string name);
        Fixture(const Fixture& other);             // deep copy (clones parameters)
        Fixture& operator=(const Fixture& other);  // deep copy

        // Append a parameter with the given (shared, interned) descriptor.
        // Footprint grows by desc->size. Returns the created parameter.
        ParamPtr add(const Parameters::Descriptor* desc);

        // Virtual dimmer bound to this fixture's color parameter (zero footprint).
        std::shared_ptr<Parameters::VDimmerParameter> addVirtualDimmer();

        // Add a virtual dimmer iff the fixture has color but no real/virtual
        // dimmer. Idempotent; called automatically when the fixture is patched.
        void ensureVirtualDimmer();

        // ---- Fragment hooks: propagate placement to the contained parameters ----
        void setStart(uint32_t st) override;
        void setBuffer(uint8_t* buf);

        // ---- identity ----
        [[nodiscard]] uint16_t fid() const { return m_fid; }
        void setFid(uint16_t fid) { m_fid = fid; }
        [[nodiscard]] uint16_t universe() const { return m_universe; }
        void setUniverse(uint16_t universe) { m_universe = universe; }

        // ---- queries ----
        [[nodiscard]] const std::string& name() const { return m_name; }
        [[nodiscard]] uint32_t footprint() const { return size; }
        [[nodiscard]] bool has(Parameters::Type t) const;
        [[nodiscard]] const std::vector<ParamPtr>& parameters() const { return m_parameters; }
        [[nodiscard]] const std::vector<ParamPtr>& parameters(Parameters::Type t) const;

        // ---- convenience (write to the buffer through the parameters) ----
        void setColor(const Utils::Colors::RGB& color);
        void setIntensity(float norm);     // dimmer if present, else virtual dimmer
        void applyVirtualDimmers();        // run after color is written for the frame

        [[nodiscard]] std::string describe() const;  // shadows Fragment::describe()
    };
}
