#include "rda5820.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include <cstdio>
#include <cmath>

namespace esphome {
namespace rda5820 {

// TODO: std::clamp isn't here yet
#define clamp(v, lo, hi) std::max(std::min(v, hi), lo)

static const char *const TAG = "rda5820";

RDA5820Component::RDA5820Component() {
  this->reset_ = false;
  memset(&this->state_, 0, sizeof(this->state_));
  // datasheet defaults
  this->state_.SOFTMUTE_EN = 1;
  this->state_.RDS_FIFO_CLR = 1;
  this->state_.VOLUME = 15;
  this->state_.LNA_PORT_SEL = (uint16_t) LnaInputPort::LNA_PORT_SEL_LNAP;
  this->state_.SEEKTH = 8;
  this->state_.INT_MODE = (uint16_t) InterruptMode::INT_MODE_UNTIL_STATUS_READ;
  this->state_.SW_LR = 1;  // datasheet says 0b10, but it's only a single bit
  this->state_.SOFTBLEND_EN = 1;
  this->state_.MODE_50M_65M = 1;
  this->state_.TH_SOFTBLEND = 19;
  this->state_.ST = 1;
  this->state_.RDSA = 0x5820;
  this->state_.RDSB = 0x5820;
  this->state_.RDSC = 0x5805;
  this->state_.RDSD = 0x5805;
  this->state_.PA_IBIT = 7;
  this->state_.RDS_DEV = 0x10;
  this->state_.PILOT_DEV = 0x0E;
  this->state_.AUDIO_DEV = 0xF0;
  // our defaults
  this->state_.SPACE = (uint16_t) ChannelSpacing::SPACE_25K;
  this->state_.BAND = (uint16_t) BandSelect::BAND_76_108;
  this->state_.CHAN = (uint16_t) lround((87.5 - 76) / 0.025);  // 87.5 MHz => 460
}

bool RDA5820Component::check_addr_(uint8_t addr) {
  if (addr == 0x01) {
    return false;
  } else if (addr >= 0x08 && addr <= 0x09) {
    return false;
  } else if (addr >= 0x10 && addr <= 0x3F) {
    return false;
  } else if (addr >= 0x42 && addr <= 0x66) {
    return false;
  } else if (addr >= 0x69) {
    return false;
  }
  return true;
}

void RDA5820Component::write_reg_(uint8_t addr) {
  if (!this->check_addr_(addr)) {
    ESP_LOGE(TAG, "%s(0x%02X) invalid register address", __func__, addr);
    return;
  }

  if (this->reset_) {
    uint16_t value = this->regs_[addr];
    if (!this->write_byte(addr, value)) {
      ESP_LOGE(TAG, "%s(0x%02X) cannot write register", __func__, addr, value);
      return;
    }
    ESP_LOGV(TAG, "%s(0x%02X) = 0x%04X", __func__, addr, value);
  } else {
    if (this->get_component_state() & COMPONENT_STATE_LOOP) {
      ESP_LOGE(TAG, "%s(0x%02X) device was not reset", __func__, addr);
    }
  }
}

bool RDA5820Component::read_reg_(uint8_t addr) {
  if (!this->check_addr_(addr)) {
    ESP_LOGE(TAG, "%s(0x%02X) invalid register address", __func__, addr);
    return false;
  }

  if (!this->read_bytes_16(addr, &this->regs_[addr])) {
    ESP_LOGE(TAG, "%s(0x%02X) cannot read register", __func__, addr);
    return false;
  }

  return true;
}

void RDA5820Component::rds_update_() {}

// overrides

void RDA5820Component::setup() {
  char buff[32] = {0};
  // snprintf(buff, sizeof(buff), "", );
  chip_id_ = buff;

  // reset
  // this->write_reg_(REG_);
  // frequency
  // this->write_reg_(REG_);

  this->publish_frequency();
  this->publish_chip_id_text_sensor();

  this->set_interval(1000, [this]() { this->rds_update_(); });
}

void RDA5820Component::dump_config() {
  ESP_LOGCONFIG(TAG, "RDA5820:");
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "failed!");
  }
  ESP_LOGCONFIG(TAG, "  Chip: %s", this->chip_id_.c_str());
  ESP_LOGCONFIG(TAG, "  Frequency: %.2f MHz", this->get_frequency());
  // TODO: ...and everything else...
  LOG_UPDATE_INTERVAL(this);
}

void RDA5820Component::update() {}

void RDA5820Component::loop() {}

// config

template<typename T> T GET_ENUM_LAST(T value) { return T::LAST; }

#define CHECK_ENUM(value) \
  if ((value) >= GET_ENUM_LAST(value)) { \
    ESP_LOGE(TAG, "%s(%d) invalid", __func__, (int) (value)); \
    return; \
  }

#define CHECK_FLOAT_RANGE(value, min_value, max_value) \
  if (!((min_value) <= (value) && (value) <= (max_value))) { \
    ESP_LOGE(TAG, "%s(%.2f) invalid (%.2f - %.2f)", __func__, value, min_value, max_value); \
    return; \
  }

#define CHECK_INT_RANGE(value, min_value, max_value) \
  if (!((min_value) <= (value) && (value) <= (max_value))) { \
    ESP_LOGE(TAG, "%s(%d) invalid (%d - %d)", __func__, value, min_value, max_value); \
    return; \
  }

#define CHECK_TEXT_RANGE(value, max_size) \
  if ((value).size() > (max_size)) { \
    ESP_LOGW(TAG, "%s(%s) trimmed (max %d characters)", __func__, (value).c_str(), max_size); \
    (value).resize(max_size); \
  }

void RDA5820Component::set_frequency(float value) {
  CHECK_FLOAT_RANGE(value, FREQ_MIN, FREQ_MAX)
  // int f = clamp((int) std::lround((value - 76) * 20), CH_FREQ_RAW_MIN, CH_FREQ_RAW_MAX);
  // this->state_.CH_UPPER = (uint8_t) (f >> 8);
  // this->write_reg_(REG_SYSTEM_ADDR);
  this->publish_frequency();
}

float RDA5820Component::get_frequency() {
  // uint16_t ch = ((uint16_t) this->state_.CH_UPPER << 8) | this->state_.CH_LOWER;
  return 0;  //(float) ch / 20 + 76;
}

std::string RDA5820Component::get_chip_id_text_sensor() { return this->chip_id_; }

template<class S, class T> void RDA5820Component::publish(S *s, T state) {
  if (s != nullptr) {
    if (!s->has_state() || s->state != state) {
      s->publish_state(state);
    }
  }
}

void RDA5820Component::publish_switch(switch_::Switch *s, bool state) {
  if (s != nullptr) {
    if (s->state != state) {  // ?
      s->publish_state(state);
    }
  }
}

void RDA5820Component::publish_select(select::Select *s, size_t index) {
  if (s != nullptr) {
    if (auto state = s->at(index)) {
      if (!s->has_state() || s->state != *state) {
        s->publish_state(*state);
      }
    }
  }
}

}  // namespace rda5820
}  // namespace esphome
