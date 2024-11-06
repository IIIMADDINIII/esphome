#pragma once

#include "esphome/components/number/number.h"
#include "../rda5820.h"
#include <cmath>

namespace esphome {
namespace rda5820 {

class FrequencyNumber : public number::Number, public Parented<RDA5820Component> {
 protected:
  void control(float value) override {
    this->publish_state(value);
    this->parent_->set_frequency(value);
  }
};

class VolumeNumber : public number::Number, public Parented<RDA5820Component> {
 protected:
  void control(float value) override {
    this->publish_state(value);
    this->parent_->set_volume(value);
  }
};

class ThSoftblendNumber : public number::Number, public Parented<RDA5820Component> {
 protected:
  void control(float value) override {
    this->publish_state(value);
    this->parent_->set_th_softblend((uint8_t) lround(value));
  }
};

class AudioDevNumber : public number::Number, public Parented<RDA5820Component> {
 protected:
  void control(float value) override {
    this->publish_state(value);
    this->parent_->set_audio_dev((uint8_t) lround(value));
  }
};

class PilotDevNumber : public number::Number, public Parented<RDA5820Component> {
 protected:
  void control(float value) override {
    this->publish_state(value);
    this->parent_->set_pilot_dev((uint8_t) lround(value));
  }
};

class PgaGainNumber : public number::Number, public Parented<RDA5820Component> {
 protected:
  void control(float value) override {
    this->publish_state(value);
    this->parent_->set_pga_gain((uint8_t) lround(value));
  }
};

class AdcGainNumber : public number::Number, public Parented<RDA5820Component> {
 protected:
  void control(float value) override {
    this->publish_state(value);
    this->parent_->set_adc_gain((uint8_t) lround(value));
  }
};

class PaGainNumber : public number::Number, public Parented<RDA5820Component> {
 protected:
  void control(float value) override {
    this->publish_state(value);
    this->parent_->set_pa_gain((uint8_t) lround(value));
  }
};

class PaIbitNumber : public number::Number, public Parented<RDA5820Component> {
 protected:
  void control(float value) override {
    this->publish_state(value);
    this->parent_->set_pa_ibit((uint8_t) lround(value));
  }
};

class PaVcomNumber : public number::Number, public Parented<RDA5820Component> {
 protected:
  void control(float value) override {
    this->publish_state(value);
    this->parent_->set_pa_vcom((uint8_t) lround(value));
  }
};

class SeekThNumber : public number::Number, public Parented<RDA5820Component> {
 protected:
  void control(float value) override {
    this->publish_state(value);
    this->parent_->set_seek_th((uint8_t) lround(value));
  }
};

class SeekThOldNumber : public number::Number, public Parented<RDA5820Component> {
 protected:
  void control(float value) override {
    this->publish_state(value);
    this->parent_->set_seek_th_old((uint8_t) lround(value));
  }
};

class RdsDeviationNumber : public number::Number, public Parented<RDA5820Component> {
 protected:
  void control(float value) override {
    this->publish_state(value);
    this->parent_->set_rds_deviation((uint8_t) lround(value));
  }
};

}  // namespace rda5820
}  // namespace esphome
