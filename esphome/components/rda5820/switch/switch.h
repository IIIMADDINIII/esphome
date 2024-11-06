#pragma once

#include "esphome/components/switch/switch.h"
#include "../rda5820.h"

namespace esphome {
namespace rda5820 {

class MonoSwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_mono(value);
  }
};

class MuteSwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_mute(value);
  }
};

class SoftmuteSwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_softmute(value);
  }
};

class BassBoostSwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_bass_boost(value);
  }
};

class AudioHiZSwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_audio_hi_z(value);
  }
};

class PowerEnableSwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_power_enable(value);
  }
};

class AfcEnableSwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_afc_enable(value);
  }
};

class RbdsEnableSwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_rbds_enable(value);
  }
};

class SoftblendEnableSwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_softblend_enable(value);
  }
};

class RclkDirectInputModeSwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_rclk_direct_input_mode(value);
  }
};

class RclkNonCalibrateModeSwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_rclk_non_calibrate_mode(value);
  }
};

class FreqModeSwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_freq_mode(value);
  }
};

class I2sEnableSwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_i2s_enable(value);
  }
};

class I2sLDelaySwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_i2s_l_delay(value);
  }
};

class I2sRDelaySwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_i2s_r_delay(value);
  }
};

class I2sSclkOEdgeSwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_i2s_sclk_o_edge(value);
  }
};

class I2sSwOEdgeSwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_i2s_sw_o_edge(value);
  }
};

class I2sWsIEdgeSwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_i2s_ws_i_edge(value);
  }
};

class I2sDataSignedSwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_i2s_data_signed(value);
  }
};

class I2sSclkIEdgeSwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_i2s_sclk_i_edge(value);
  }
};

class I2sSwLrSwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_i2s_sw_lr(value);
  }
};

class RdsEnableSwitch : public switch_::Switch, public Parented<RDA5820Component> {
 protected:
  void write_state(bool value) override {
    this->publish_state(value);
    this->parent_->set_rds_enable(value);
  }
};

}  // namespace rda5820
}  // namespace esphome
