#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"

namespace esphome {
namespace store_yaml {
class StoreYamlComponent : public Component {
  bool show_in_dump_config_{false};
  std::string yaml_;

 public:
  void dump_config() override;
  void set_show_in_dump_config(bool show) { this->show_in_dump_config_ = show; }
  void set_yaml(const std::string &yaml) { this->yaml_ = yaml; }
  std::string get_yaml() const;
  void log(bool dump_config = false) const;
};

template<typename... Ts> class LogAction : public Action<Ts...>, public Parented<StoreYamlComponent> {
 public:
  void play(Ts... x) override { this->parent_->log(); }
};

}  // namespace store_yaml
}  // namespace esphome
