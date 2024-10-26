#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
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
  std::string rds_station_;
  std::string rds_text_;

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

  void seek(bool seek_up, bool wrap_around);

  RDA5820_SUB_SELECT(work_mode, WorkMode)
  RDA5820_SUB_SELECT(lna_port_sel, LnaInputPort)
  RDA5820_SUB_NUMBER(frequency, float)
  RDA5820_SUB_SELECT(de_emphasis, DeEmphasis)
  RDA5820_SUB_NUMBER(volume, float)
  RDA5820_SUB_SWITCH(mono)
  RDA5820_SUB_SWITCH(mute)
  RDA5820_SUB_SWITCH(softmute)
  RDA5820_SUB_SWITCH(bass_boost)
  RDA5820_SUB_SWITCH(audio_hi_z)
  RDA5820_SUB_SWITCH(power_enable)
  RDA5820_SUB_SWITCH(afc_enable)
  RDA5820_SUB_SWITCH(rbds_enable)
  RDA5820_SUB_SWITCH(softblend_enable)
  RDA5820_SUB_NUMBER(th_softblend, uint8_t)
  RDA5820_SUB_SELECT(clk_mode, ClkMode)
  RDA5820_SUB_SWITCH(rclk_direct_input_mode)
  RDA5820_SUB_SWITCH(rclk_non_calibrate_mode)
  RDA5820_SUB_NUMBER(audio_dev, uint8_t)  // TODO: no unit in datasheet, default 240
  RDA5820_SUB_NUMBER(pilot_dev, uint8_t)  // TODO: no unit in datasheet, default 14
  RDA5820_SUB_NUMBER(pga_gain, uint8_t)
  RDA5820_SUB_NUMBER(adc_gain, uint8_t)
  RDA5820_SUB_NUMBER(pa_gain, uint8_t)
  RDA5820_SUB_NUMBER(pa_ibit, uint8_t)
  RDA5820_SUB_NUMBER(pa_vcom, uint8_t)
  RDA5820_SUB_SELECT(seek_mode, SeekMode)
  RDA5820_SUB_NUMBER(seek_th, uint8_t)
  RDA5820_SUB_NUMBER(seek_th_old, uint8_t)
  RDA5820_SUB_SWITCH(freq_mode)  // is this a read-only status bit? what is this?
  RDA5820_SUB_SWITCH(i2s_enable)
  RDA5820_SUB_SWITCH(i2s_l_delay)
  RDA5820_SUB_SWITCH(i2s_r_delay)
  RDA5820_SUB_SWITCH(i2s_sclk_o_edge)
  RDA5820_SUB_SWITCH(i2s_sw_o_edge)
  RDA5820_SUB_SELECT(i2s_sw_cnt, I2sWordSelect)
  RDA5820_SUB_SWITCH(i2s_ws_i_edge)
  RDA5820_SUB_SWITCH(i2s_data_signed)
  RDA5820_SUB_SWITCH(i2s_sclk_i_edge)
  RDA5820_SUB_SWITCH(i2s_sw_lr)
  RDA5820_SUB_SELECT(i2s_mode, I2sMode)
  RDA5820_SUB_SWITCH(rds_enable)
  RDA5820_SUB_NUMBER(rds_deviation, uint8_t)  // TODO: no unit in datasheet, default 16
  RDA5820_SUB_TEXT(rds_station)
  RDA5820_SUB_TEXT(rds_text)
  RDA5820_SUB_SELECT(gpio1, Gpio1Control)
  RDA5820_SUB_SELECT(gpio2, Gpio2Control)
  RDA5820_SUB_SELECT(gpio3, Gpio3Control)
  RDA5820_SUB_TEXT_SENSOR(chip_id)
  RDA5820_SUB_SENSOR(frequency)
  RDA5820_SUB_SENSOR(rssi)
  RDA5820_SUB_BINARY_SENSOR(stereo)
  RDA5820_SUB_BINARY_SENSOR(seek_fail)
  RDA5820_SUB_BINARY_SENSOR(seek_tune_complete)
  RDA5820_SUB_BINARY_SENSOR(fm_ready)
  RDA5820_SUB_BINARY_SENSOR(fm_is_station)
  RDA5820_SUB_BINARY_SENSOR(blk_e_found)
  RDA5820_SUB_BINARY_SENSOR(rds_decoder_sync)
  // TODO: RDS sensors

  // TODO: gpio output pins
};

template<typename... Ts> class SetFrequencyAction : public Action<Ts...>, public Parented<RDA5820Component> {
  TEMPLATABLE_VALUE(float, frequency)
  void play(Ts... x) override { this->parent_->set_frequency(this->frequency_.value(x...)); }
};

template<typename... Ts> class SeekAction : public Action<Ts...>, public Parented<RDA5820Component> {
  TEMPLATABLE_VALUE(bool, seek_up)
  TEMPLATABLE_VALUE(bool, wrap_around)
  void play(Ts... x) override { this->parent_->seek(this->seek_up_.value(x...), this->wrap_around_.value(x...)); }
};

}  // namespace rda5820
}  // namespace esphome
