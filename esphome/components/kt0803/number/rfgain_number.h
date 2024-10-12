#pragma once

#include "esphome/components/number/number.h"
#include "../kt0803.h"

namespace esphome {
namespace kt0803 {

class RfGainNumber : public number::Number, public Parented<KT0803Component> {
 public:
  RfGainNumber() = default;

 protected:
  void control(float value) override;
};

}  // namespace kt0803
}  // namespace esphome
