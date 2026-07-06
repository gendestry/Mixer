#pragma once
#include "Attribute.h"
#include <string>
#include <utility>

namespace Core::Attributes {
class Attribute {
  std::string m_name;
  int m_id = 0;

public:
  Attribute() = default;
  explicit Attribute(std::string name) : m_name(std::move(name)) {}
  virtual ~Attribute() = default;

  [[nodiscard]] const std::string &name() const { return m_name; }
  void setName(std::string name) { m_name = std::move(name); }

  [[nodiscard]] int id() const { return m_id; }
  void setId(int id) { m_id = id; }

  [[nodiscard]] virtual std::string describe() const = 0;
};
} // namespace Core::Attributes
