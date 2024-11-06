#pragma once

#include "esphome/components/select/select.h"
#include "../rda5820.h"

namespace esphome {
namespace rda5820 {

class WorkModeSelect : public select::Select, public Parented<RDA5820Component> {
 protected:
  void control(const std::string &value) override {
    this->publish_state(value);
    if (auto index = this->active_index()) {
      this->parent_->set_work_mode((WorkMode) *index);
    }
  }
};

class LnaPortSelSelect : public select::Select, public Parented<RDA5820Component> {
 protected:
  void control(const std::string &value) override {
    this->publish_state(value);
    if (auto index = this->active_index()) {
      this->parent_->set_lna_port_sel((LnaInputPort) *index);
    }
  }
};

class DeEmphasisSelect : public select::Select, public Parented<RDA5820Component> {
 protected:
  void control(const std::string &value) override {
    this->publish_state(value);
    if (auto index = this->active_index()) {
      this->parent_->set_de_emphasis((DeEmphasis) *index);
    }
  }
};

class ClkModeSelect : public select::Select, public Parented<RDA5820Component> {
 protected:
  void control(const std::string &value) override {
    this->publish_state(value);
    if (auto index = this->active_index()) {
      this->parent_->set_clk_mode((ClkMode) *index);
    }
  }
};

class SeekModeSelect : public select::Select, public Parented<RDA5820Component> {
 protected:
  void control(const std::string &value) override {
    this->publish_state(value);
    if (auto index = this->active_index()) {
      this->parent_->set_seek_mode((SeekMode) *index);
    }
  }
};

class I2sSwCntSelect : public select::Select, public Parented<RDA5820Component> {
 protected:
  void control(const std::string &value) override {
    this->publish_state(value);
    if (auto index = this->active_index()) {
      this->parent_->set_i2s_sw_cnt((I2sWordSelect) *index);
    }
  }
};

class I2sModeSelect : public select::Select, public Parented<RDA5820Component> {
 protected:
  void control(const std::string &value) override {
    this->publish_state(value);
    if (auto index = this->active_index()) {
      this->parent_->set_i2s_mode((I2sMode) *index);
    }
  }
};

class Gpio1Select : public select::Select, public Parented<RDA5820Component> {
 protected:
  void control(const std::string &value) override {
    this->publish_state(value);
    if (auto index = this->active_index()) {
      this->parent_->set_gpio1((Gpio1Control) *index);
    }
  }
};

class Gpio2Select : public select::Select, public Parented<RDA5820Component> {
 protected:
  void control(const std::string &value) override {
    this->publish_state(value);
    if (auto index = this->active_index()) {
      this->parent_->set_gpio2((Gpio2Control) *index);
    }
  }
};

class Gpio3Select : public select::Select, public Parented<RDA5820Component> {
 protected:
  void control(const std::string &value) override {
    this->publish_state(value);
    if (auto index = this->active_index()) {
      this->parent_->set_gpio3((Gpio3Control) *index);
    }
  }
};

}  // namespace rda5820
}  // namespace esphome
