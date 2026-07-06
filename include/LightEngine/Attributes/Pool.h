#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <type_traits>
#include <utility>

#include "LightEngine/Attributes/Attribute.h"

//
// Pool<T>: a generic container of poolable Attributes keyed by an integer id
// (the pool "cell" / slot the user types). One template serves every pool -
// Groups, Colour presets, Position presets, FX, etc. store() overwrites by id
// and stamps the id onto the item; get() returns nullptr for an empty cell.
//
// T must derive from Attribute (store() calls setId, rename() calls setName).
//
namespace LightEngine::Attributes
{
    template <typename T>
    class Pool
    {
        static_assert(std::is_base_of_v<Attribute, T>, "Pool<T>: T must derive from Attribute");

        std::map<int, T> m_items;

    public:
        // Store (or replace) the item at `id`, stamping `id` as its id.
        T& store(int id, T item)
        {
            auto [it, inserted] = m_items.insert_or_assign(id, std::move(item));
            it->second.setId(id);
            return it->second;
        }

        [[nodiscard]] T* get(int id)
        {
            const auto it = m_items.find(id);
            return it != m_items.end() ? &it->second : nullptr;
        }
        [[nodiscard]] const T* get(int id) const
        {
            const auto it = m_items.find(id);
            return it != m_items.end() ? &it->second : nullptr;
        }

        [[nodiscard]] bool contains(int id) const { return m_items.contains(id); }
        bool remove(int id) { return m_items.erase(id) > 0; }

        bool rename(int id, std::string name)
        {
            if (T* item = get(id)) { item->setName(std::move(name)); return true; }
            return false;
        }

        void clear() { m_items.clear(); }

        [[nodiscard]] std::size_t size() const { return m_items.size(); }
        [[nodiscard]] bool empty() const { return m_items.empty(); }

        // Ordered by id, for UI listing / iteration.
        [[nodiscard]] const std::map<int, T>& items() const { return m_items; }
    };
}
