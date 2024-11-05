#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "hlw8032defs.h"
#include <deque>

namespace esphome {
namespace hlw8032 {

class HLW8032Component : public Component, public uart::UARTDevice {
  std::deque<uint8_t> buff_;
  struct register_set regs_;
  uint32_t last_pf_;
  float voltage_coef_;
  float current_coef_;

  void publish_();
  template<class S, class T> void publish_(S *s, T state);

 public:
  HLW8032Component();

  void setup() override;
  void loop() override;
  float get_setup_priority() const override;
  void dump_config() override;
  // void update() override;

  void set_voltage_coef(float value) { this->voltage_coef_ = value; }
  void set_current_coef(float value) { this->current_coef_ = value; }

  SUB_SENSOR(voltage)
  SUB_SENSOR(current)
  SUB_SENSOR(power)
  SUB_SENSOR(energy)
  SUB_SENSOR(apparent_power)
  SUB_SENSOR(reactive_power)
  SUB_SENSOR(power_factor)

 protected:
};

}  // namespace hlw8032
}  // namespace esphome
