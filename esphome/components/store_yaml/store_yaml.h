#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"

namespace esphome {
namespace store_yaml {
class StoreYamlComponent : public Component {
  std::string yaml_;

 public:
  void set(const std::string &yaml);
  std::string get() const;
  void dump() const;
};

template<typename... Ts> class DumpAction : public Action<Ts...>, public Parented<StoreYamlComponent> {
 public:
  void play(Ts... x) override { this->parent_->dump(); }
};

}  // namespace store_yaml
}  // namespace esphome
