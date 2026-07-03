#pragma once

#include <string>
#include <unordered_map>

#include "LightEngine/Fixture/Fixture.h"
#include "LightEngine/Fixture/Parameters/Descriptor.h"

//
// FixtureLibrary: the catalogue of fixture *definitions* (personalities). Each
// entry is a LightEngine::Fixture template built once (programmatically now, from a
// file later) and copied whenever it is patched into a universe.
//
// It also owns a DescriptorStore so parameter layouts built at runtime (from a
// file) are interned here and outlive the fixtures that reference them.
//
namespace LightEngine::Engine::Components
{
    class FixtureLibrary
    {
        std::unordered_map<std::string, LightEngine::Fixture> m_fixtures;  // name -> definition
        Parameters::DescriptorStore                    m_descriptors;

    public:
        // Create (or fetch) an empty definition under `name` and return it by
        // reference so the caller can stack parameters onto it.
        LightEngine::Fixture& define(const std::string& name);

        // Register a fully-built definition (keyed by its own name, or explicitly).
        void add(const LightEngine::Fixture& fixture);
        void add(const std::string& name, const LightEngine::Fixture& fixture);

        [[nodiscard]] const LightEngine::Fixture* get(const std::string& name) const;
        [[nodiscard]] bool has(const std::string& name) const;
        [[nodiscard]] std::size_t size() const { return m_fixtures.size(); }

        // Interning point for runtime/file-built parameter layouts.
        [[nodiscard]] Parameters::DescriptorStore& descriptors() { return m_descriptors; }

        [[nodiscard]] std::string describe() const;
    };
}
