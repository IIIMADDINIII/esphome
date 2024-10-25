#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/number/number.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/select/select.h"
#include "esphome/components/text/text.h"
#include <string>
#include "rda5820defs.h"
#include "rda5820sub.h"

namespace esphome {
namespace rda5820 {

class RDA5820Component : public PollingComponent, public i2c::I2CDevice {
  std::string chip_id_;
  bool reset_;
  union {
    struct RDA5820State state_;
    uint16_t regs_[sizeof(struct RDA5820State) / sizeof(uint16_t)];
  };

  bool check_addr_(uint8_t addr);
  void write_reg_(uint8_t addr);
  bool read_reg_(uint8_t addr);
  void rds_update_();

  template<class S, class T> void publish(S *s, T state);
  // template specialization here is not supported by the compiler yet
  void publish_switch(switch_::Switch *s, bool state);
  void publish_select(select::Select *s, size_t index);

 public:
  RDA5820Component();

  void setup() override;
  void dump_config() override;
  void update() override;
  void loop() override;

  RDA5820_SUB_NUMBER(frequency, float)
  RDA5820_SUB_TEXT_SENSOR(chip_id)
};

template<typename... Ts> class SetFrequencyAction : public Action<Ts...>, public Parented<RDA5820Component> {
  TEMPLATABLE_VALUE(float, frequency)
  void play(Ts... x) override { this->parent_->set_frequency(this->frequency_.value(x...)); }
};

}  // namespace rda5820
}  // namespace esphome
