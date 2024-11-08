#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"

extern const char ESPHOME_YAML[];

namespace esphome {
namespace store_yaml {
class StoreYamlComponent : public Component {
  bool show_in_dump_config_{false};
  const char *yaml_;

 public:
  void dump_config() override;
  void set_show_in_dump_config(bool show) { this->show_in_dump_config_ = show; }
  void set_yaml(const char *yaml) { this->yaml_ = yaml; }
  const char *get_yaml() const;
  void log(bool dump_config = false) const;
};

template<typename... Ts> class LogAction : public Action<Ts...>, public Parented<StoreYamlComponent> {
 public:
  void play(Ts... x) override { this->parent_->log(); }
};

}  // namespace store_yaml
}  // namespace esphome
