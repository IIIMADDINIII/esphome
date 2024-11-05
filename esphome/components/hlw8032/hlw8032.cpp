#include "hlw8032.h"
#include "esphome/core/log.h"

namespace esphome {
namespace hlw8032 {

static const char *const TAG = "hlw8032";

HLW8032Component::HLW8032Component() {
  memset((void *) &this->regs_, 0, sizeof(this->regs_));
  this->last_pf_ = UINT32_MAX;
  this->voltage_coef_ = 1.88f;
  this->current_coef_ = 1.00f;
}

void HLW8032Component::setup() {}

void HLW8032Component::loop() {
  while (this->available()) {
    uint8_t b = 0;
    if (!this->read_byte(&b))
      return;
    this->buff_.push_back(b);

    if (this->buff_.size() < 24)
      continue;

    uint8_t state = this->buff_[0];
    uint8_t magic = this->buff_[1];

    this->buff_.pop_front();

    if (state == 0xaa || !(state == 0x55 || (state & 0xf0) == 0xf0) || magic != 0x5A) {
      continue;
    }

    ESP_LOGV(TAG, "%s", format_hex_pretty(std::vector<uint8_t>(this->buff_.begin(), this->buff_.end())).c_str());

    uint8_t chksum = 0;
    for (int i = 1; i < 22; i++) {
      chksum += this->buff_[i];
    }
    if (chksum != this->buff_[22]) {
      ESP_LOGD(TAG, "bad checksum");
      continue;
    }

    this->buff_.pop_front();

    for (int i = 0; i < 18; i++) {
      uint32_t &var = this->regs_.vars[i / 3];
      var = ((var << 8) | this->buff_[i]) & 0xFFFFFF;
    }

    this->regs_.state = state;
    this->regs_.upd = this->buff_[18];
    this->regs_.pf = ((uint16_t) this->buff_[19] << 8) | this->buff_[20];

    ESP_LOGV(TAG, "error %d", this->regs_.error);
    ESP_LOGV(TAG, "overflow p %d c %d v %d", this->regs_.p_overflow, this->regs_.c_overflow, this->regs_.v_overflow);
    ESP_LOGV(TAG, "valid p %d c %d v %d", this->regs_.p_valid, this->regs_.c_valid, this->regs_.v_valid);
    ESP_LOGV(TAG, "voltage %06X/%06X", this->regs_.vp, this->regs_.v);
    ESP_LOGV(TAG, "current %06X/%06X", this->regs_.cp, this->regs_.c);
    ESP_LOGV(TAG, "power %06X/%06X", this->regs_.pp, this->regs_.p);
    ESP_LOGV(TAG, "pulse count %05X ", ((uint32_t) this->regs_.pf_carry << 16) | this->regs_.pf);

    this->publish_();

    this->buff_.clear();
  }
}

float HLW8032Component::get_setup_priority() const { return setup_priority::DATA; }

void HLW8032Component::dump_config() {
  ESP_LOGCONFIG(TAG, "HLW8032:");
  ESP_LOGCONFIG("  ", "Voltage Coefficient", this->voltage_coef_);
  ESP_LOGCONFIG("  ", "Current Coefficient", this->current_coef_);
  LOG_SENSOR("  ", "Voltage", this->voltage_sensor_);
  LOG_SENSOR("  ", "Current", this->current_sensor_);
  LOG_SENSOR("  ", "Power", this->power_sensor_);
  LOG_SENSOR("  ", "Energy", this->energy_sensor_);
  LOG_SENSOR("  ", "Apparent Power", this->apparent_power_sensor_);
  LOG_SENSOR("  ", "Reactive Power", this->reactive_power_sensor_);
  LOG_SENSOR("  ", "Power Factor", this->power_factor_sensor_);
  this->check_uart_settings(4800, 1, uart::UART_CONFIG_PARITY_EVEN, 8);
}

void HLW8032Component::publish_() {
  float voltage = 0;
  float current = 0;
  float power = 0;
  float apparent_power = 0;

  if (this->regs_.v_valid) {
    voltage = this->voltage_coef_ * this->regs_.vp / this->regs_.v;
    this->publish_(this->voltage_sensor_, voltage);
  }

  if (this->regs_.c_valid) {
    current = this->current_coef_ * this->regs_.cp / this->regs_.c;
    this->publish_(this->current_sensor_, current);
  }

  if (this->regs_.p_valid) {
    power = this->voltage_coef_ * this->current_coef_ * this->regs_.pp / this->regs_.p;
    this->publish_(this->power_sensor_, power);
  }

  if (this->regs_.v_valid && this->regs_.c_valid) {
    apparent_power = voltage * current;
    this->publish_(this->apparent_power_sensor_, apparent_power);
  }

  if (this->regs_.p_valid && this->regs_.v_valid && this->regs_.c_valid) {
    this->publish_(this->power_factor_sensor_, power / apparent_power);
  }

  if (this->regs_.p_valid) {
    uint32_t pf = ((uint32_t) this->regs_.pf_carry << 16) | this->regs_.pf;
    if (pf >= this->last_pf_) {
      // as in the datasheet
      float pf_per_kwh = 1.0e9 * 3600 / (this->regs_.pp * this->voltage_coef_ * this->current_coef_);
      float energy = (float) (pf - this->last_pf_) / pf_per_kwh;
      this->publish_(this->energy_sensor_, energy);
    }
    this->last_pf_ = this->regs_.pf;
  }
}

template<class S, class T> void HLW8032Component::publish_(S *s, T state) {
  if (s != nullptr) {
    if (!s->has_state() || s->state != state) {
      s->publish_state(state);
    }
  }
}

}  // namespace hlw8032
}  // namespace esphome
