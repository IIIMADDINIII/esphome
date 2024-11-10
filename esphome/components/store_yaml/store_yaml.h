#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "esphome/core/hal.h"
#include <memory>
#include "decompressor.h"

extern const uint8_t ESPHOME_YAML[] PROGMEM;
extern const size_t ESPHOME_YAML_SIZE;

namespace esphome {
namespace store_yaml {

class StoreYamlComponent : public Component {
  std::unique_ptr<RowDecompressor> dec_;
  bool show_in_dump_config_{false};

 public:
  void dump_config() override;
  void loop() override;
  void log();
  void set_show_in_dump_config(bool show) { this->show_in_dump_config_ = show; }
};

template<typename... Ts> class LogAction : public Action<Ts...>, public Parented<StoreYamlComponent> {
 public:
  void play(Ts... x) override { this->parent_->log(); }
};

}  // namespace store_yaml
}  // namespace esphome
