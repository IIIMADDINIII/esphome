#pragma once

#include "esphome/components/text/text.h"
#include "../rda5820.h"

namespace esphome {
namespace rda5820 {

class RdsStationText : public text::Text, public Parented<RDA5820Component> {
 protected:
  void control(const std::string &value) override {
    this->publish_state(value);
    this->parent_->set_rds_station(value);
  }
};

class RdsTextText : public text::Text, public Parented<RDA5820Component> {
 protected:
  void control(const std::string &value) override {
    this->publish_state(value);
    this->parent_->set_rds_text(value);
  }
};

}  // namespace rda5820
}  // namespace esphome
