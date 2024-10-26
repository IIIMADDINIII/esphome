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
  this->state_.SEEK_TH = 8;
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

  if (addr == 0x02 && this->state_.SOFT_RESET == 1) {
    this->state_.SOFT_RESET = 0;
  }

  if (addr == 0x02 && this->state_.SEEK == 1) {
    this->state_.SEEK = 0;
  }

  // TODO: move STC to a binary_sensor and let this go async? (same for SEEK=1)
  if (addr == 0x03 && this->state_.TUNE == 1) {
    // wait STC flag
    do {
      this->read_reg_(0x0A);
    } while (this->state_.STC == 0);
    // this may not be needed, but the datasheet says TUNE is also reset, make sure it was
    do {
      this->read_reg_(0x03);
    } while (this->state_.TUNE == 1);
  }
}

bool RDA5820Component::read_reg_(uint8_t addr) {
  if (!this->check_addr_(addr)) {
    ESP_LOGE(TAG, "%s(0x%02X) invalid register address", __func__, addr);
    return false;
  }

  if (!this->read_bytes_16(addr, &this->regs_[addr], 1)) {
    ESP_LOGE(TAG, "%s(0x%02X) cannot read register", __func__, addr);
    return false;
  }

  return true;
}

void RDA5820Component::rds_update_() {}

// overrides

void RDA5820Component::setup() {
  this->read_reg_(0x00);
  if (this->state_.CHIP_ID != 0x5820 && this->state_.CHIP_ID != 0x5802) {  // 0x5802 is receive only
    ESP_LOGE(TAG, "chip id is not supported %04X", this->state_.CHIP_ID);
    this->mark_failed();
    return;
  }

  char buff[32] = {0};
  snprintf(buff, sizeof(buff), "RDA%04X", this->state_.CHIP_ID);
  chip_id_ = buff;

  // power up (after reset? after setup? can we switch power on/off without losing the register values?)
  this->state_.ENABLE = 1;
  this->write_reg_(0x02);

  // reset
  this->state_.SOFT_RESET = 1;
  this->write_reg_(0x02);

  // setup
  this->write_reg_(0x02);
  this->write_reg_(0x03);
  this->write_reg_(0x04);
  this->write_reg_(0x05);
  this->write_reg_(0x06);
  this->write_reg_(0x07);
  this->write_reg_(0x40);
  this->write_reg_(0x41);
  this->write_reg_(0x67);
  this->write_reg_(0x68);

  // tune
  this->state_.TUNE = 1;
  this->write_reg_(0x03);

  this->publish_work_mode();
  this->publish_lna_port_sel();
  this->publish_frequency();
  this->publish_de_emphasis();
  this->publish_volume();
  this->publish_mono();
  this->publish_mute();
  this->publish_softmute();
  this->publish_bass_boost();
  this->publish_audio_hi_z();
  this->publish_power_enable();
  this->publish_afc_enable();
  this->publish_rbds_enable();
  this->publish_softblend_enable();
  this->publish_th_softblend();
  this->publish_clk_mode();
  this->publish_rclk_direct_input_mode();
  this->publish_rclk_non_calibrate_mode();
  this->publish_audio_dev();
  this->publish_pilot_dev();
  this->publish_pga_gain();
  this->publish_adc_gain();
  this->publish_pa_gain();
  this->publish_pa_ibit();
  this->publish_pa_vcom();
  this->publish_seek_mode();
  this->publish_seek_th();
  this->publish_seek_th_old();
  this->publish_freq_mode();
  this->publish_i2s_enable();
  this->publish_i2s_l_delay();
  this->publish_i2s_r_delay();
  this->publish_i2s_sclk_o_edge();
  this->publish_i2s_sw_o_edge();
  this->publish_i2s_sw_cnt();
  this->publish_i2s_ws_i_edge();
  this->publish_i2s_data_signed();
  this->publish_i2s_sclk_i_edge();
  this->publish_i2s_sw_lr();
  this->publish_i2s_mode();
  this->publish_rds_enable();
  this->publish_rds_deviation();
  this->publish_rds_station();
  this->publish_rds_text();
  this->publish_gpio1();
  this->publish_gpio2();
  this->publish_gpio3();
  this->publish_chip_id_text_sensor();
  this->publish_frequency_sensor();
  this->publish_rssi_sensor();
  this->publish_stereo_binary_sensor();
  this->publish_seek_fail_binary_sensor();
  this->publish_seek_tune_complete_binary_sensor();
  this->publish_fm_ready_binary_sensor();
  this->publish_fm_is_station_binary_sensor();
  this->publish_blk_e_found_binary_sensor();
  this->publish_rds_decoder_sync_binary_sensor();

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

void RDA5820Component::update() {
  this->publish_rssi_sensor();
}

void RDA5820Component::loop() {
  this->read_reg_(0x07);
  this->read_reg_(0x08);
  this->publish_frequency_sensor();
  //  this->publish_rssi_sensor();  // may change too fast
  this->publish_stereo_binary_sensor();
  this->publish_seek_fail_binary_sensor();
  this->publish_seek_tune_complete_binary_sensor();
  this->publish_fm_ready_binary_sensor();
  this->publish_fm_is_station_binary_sensor();
  this->publish_blk_e_found_binary_sensor();
  this->publish_rds_decoder_sync_binary_sensor();
}

void RDA5820Component::seek(bool seek_up, bool wrap_around) {
  this->state_.SEEK = 1;
  this->state_.SKMODE = wrap_around ? 0 : 1;
  this->state_.SEEKUP = seek_up ? 1 : 0;
  this->write_reg_(0x02);
  // TODO: publish stc
}

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

void RDA5820Component::set_work_mode(WorkMode value) {
  CHECK_ENUM(value)
  this->state_.WORK_MODE = (uint16_t) value;
  this->write_reg_(0x41);
  this->publish_work_mode();
}

WorkMode RDA5820Component::get_work_mode() { return (WorkMode) this->state_.WORK_MODE; }

void RDA5820Component::set_lna_port_sel(LnaInputPort value) {
  CHECK_ENUM(value)
  this->state_.LNA_PORT_SEL = (uint16_t) value;
  this->write_reg_(0x05);
  this->publish_lna_port_sel();
}

LnaInputPort RDA5820Component::get_lna_port_sel() { return (LnaInputPort) this->state_.LNA_PORT_SEL; }

static const uint8_t BAND[] = {87, 76, 76, 65};
static const uint8_t SPACE[] = {10, 5, 20, 40};

void RDA5820Component::set_frequency(float value) {
  CHECK_FLOAT_RANGE(value, FREQUENCY_MIN, FREQUENCY_MAX)
  CHECK_ENUM((ChannelSpacing) this->state_.SPACE)
  uint16_t space = SPACE[this->state_.SPACE];
  this->state_.CHAN = (uint16_t) lround(value * space);
  this->state_.SPACE = (uint16_t) ChannelSpacing::SPACE_25K;
  if (this->state_.CHAN >= 87 * space) {
    this->state_.BAND = (uint16_t) BandSelect::BAND_87_108;
    this->state_.CHAN -= 87 * space;
  } else if (this->state_.CHAN >= 76 * space) {
    this->state_.BAND = (uint16_t) BandSelect::BAND_76_91;
    this->state_.CHAN -= 76 * space;
  } else if (this->state_.CHAN >= 65 * space) {
    this->state_.BAND = (uint16_t) BandSelect::BAND_65_76_OR_50_65;
    this->state_.CHAN -= 65 * space;
    this->state_.MODE_50M_65M = 0;
  } else {
    this->state_.BAND = (uint16_t) BandSelect::BAND_65_76_OR_50_65;
    this->state_.CHAN -= 50 * space;
    this->state_.MODE_50M_65M = 1;
  }
  this->write_reg_(0x05);
  this->write_reg_(0x07);
  this->publish_frequency();
}

float RDA5820Component::get_frequency() {
  uint16_t band = BAND[this->state_.BAND];
  uint16_t space = SPACE[this->state_.SPACE];
  if ((BandSelect) this->state_.BAND == BandSelect::BAND_65_76_OR_50_65 && this->state_.MODE_50M_65M == 1) {
    band = 50;
  }
  return (float) this->state_.CHAN / space - band;
}

void RDA5820Component::set_de_emphasis(DeEmphasis value) {
  CHECK_ENUM(value)
  this->state_.DE = (uint16_t) value;
  this->write_reg_(0x04);
  this->publish_de_emphasis();
}

DeEmphasis RDA5820Component::get_de_emphasis() { return (DeEmphasis) this->state_.DE; }

void RDA5820Component::set_volume(float value) {
  CHECK_FLOAT_RANGE(value, VOLUME_MIN, VOLUME_MAX)
  this->state_.VOLUME = (uint16_t) lround(value * 15);
  this->write_reg_(0x05);
  this->publish_volume();
}

float RDA5820Component::get_volume() { return (float) this->state_.VOLUME / 15; }

void RDA5820Component::set_mono(bool value) {
  this->state_.MONO = value ? 1 : 0;
  this->write_reg_(0x02);
  this->publish_mono();
}

bool RDA5820Component::get_mono() { return this->state_.MONO == 1; }

void RDA5820Component::set_mute(bool value) {
  this->state_.DMUTE = value ? 0 : 1;
  this->write_reg_(0x02);
  this->publish_mute();
}

bool RDA5820Component::get_mute() { return this->state_.DMUTE == 0; }

void RDA5820Component::set_softmute(bool value) {
  this->state_.SOFTMUTE_EN = value ? 1 : 0;
  this->write_reg_(0x04);
  this->publish_softmute();
}

bool RDA5820Component::get_softmute() { return this->state_.SOFTMUTE_EN == 1; }

void RDA5820Component::set_bass_boost(bool value) {
  this->state_.BASS = value ? 1 : 0;
  this->write_reg_(0x02);
  this->publish_bass_boost();
}

bool RDA5820Component::get_bass_boost() { return this->state_.BASS == 1; }

void RDA5820Component::set_audio_hi_z(bool value) {
  this->state_.DHIZ = value ? 0 : 1;
  this->write_reg_(0x02);
  this->publish_audio_hi_z();
}

bool RDA5820Component::get_audio_hi_z() { return this->state_.DHIZ == 0; }

void RDA5820Component::set_power_enable(bool value) {
  this->state_.ENABLE = value ? 1 : 0;
  this->write_reg_(0x02);
  this->publish_power_enable();
}

bool RDA5820Component::get_power_enable() { return this->state_.ENABLE == 1; }

void RDA5820Component::set_afc_enable(bool value) {
  this->state_.AFCD = value ? 0 : 1;
  this->write_reg_(0x04);
  this->publish_afc_enable();
}

bool RDA5820Component::get_afc_enable() { return this->state_.AFCD == 0; }

void RDA5820Component::set_rbds_enable(bool value) {
  this->state_.RBDS = value ? 1 : 0;
  this->write_reg_(0x04);
  this->publish_rbds_enable();
}

bool RDA5820Component::get_rbds_enable() { return this->state_.RBDS == 1; }

void RDA5820Component::set_softblend_enable(bool value) {
  this->state_.SOFTBLEND_EN = value ? 1 : 0;
  this->write_reg_(0x07);
  this->publish_softblend_enable();
}

bool RDA5820Component::get_softblend_enable() { return this->state_.SOFTBLEND_EN == 1; }

void RDA5820Component::set_th_softblend(uint8_t value) {
  CHECK_INT_RANGE(value, TH_SOFTBLEND_MIN, TH_SOFTBLEND_MAX)
  this->state_.TH_SOFTBLEND = value;
  this->write_reg_(0x07);
  this->publish_th_softblend();
}

uint8_t RDA5820Component::get_th_softblend() { return (uint8_t) this->state_.TH_SOFTBLEND; }

void RDA5820Component::set_clk_mode(ClkMode value) {
  CHECK_ENUM(value)
  this->state_.CLK_MODE = (uint16_t) value;
  this->write_reg_(0x02);
  this->publish_clk_mode();
}

ClkMode RDA5820Component::get_clk_mode() { return (ClkMode) this->state_.CLK_MODE; }

void RDA5820Component::set_rclk_direct_input_mode(bool value) {
  this->state_.RCLK_DIRECT_INPUT_MODE = value ? 1 : 0;
  this->write_reg_(0x02);
  this->publish_rclk_direct_input_mode();
}

bool RDA5820Component::get_rclk_direct_input_mode() { return this->state_.RCLK_DIRECT_INPUT_MODE == 1; }

void RDA5820Component::set_rclk_non_calibrate_mode(bool value) {
  this->state_.RCLK_NON_CALIBRATE_MODE = value ? 1 : 0;
  this->write_reg_(0x02);
  this->publish_rclk_non_calibrate_mode();
}

bool RDA5820Component::get_rclk_non_calibrate_mode() { return this->state_.RCLK_NON_CALIBRATE_MODE == 1; }

void RDA5820Component::set_audio_dev(uint8_t value) {
  CHECK_INT_RANGE(value, AUDIO_DEV_MIN, AUDIO_DEV_MAX)
  this->state_.AUDIO_DEV = value;
  this->write_reg_(0x68);
  this->publish_audio_dev();
}

uint8_t RDA5820Component::get_audio_dev() { return (uint8_t) this->state_.AUDIO_DEV; }

void RDA5820Component::set_pilot_dev(uint8_t value) {
  CHECK_INT_RANGE(value, PILOT_DEV_MIN, PILOT_DEV_MAX)
  this->state_.PILOT_DEV = value;
  this->write_reg_(0x67);
  this->publish_pilot_dev();
}

uint8_t RDA5820Component::get_pilot_dev() { return (uint8_t) this->state_.PILOT_DEV; }

void RDA5820Component::set_pga_gain(uint8_t value) {
  CHECK_INT_RANGE(value, PGA_GAIN_MIN, PGA_GAIN_MAX)
  this->state_.PGA_GAIN = value;
  this->write_reg_(0x68);
  this->publish_pga_gain();
}

uint8_t RDA5820Component::get_pga_gain() { return (uint8_t) this->state_.PGA_GAIN; }

void RDA5820Component::set_adc_gain(uint8_t value) {
  CHECK_INT_RANGE(value, ADC_GAIN_MIN, ADC_GAIN_MAX)
  this->state_.ADC_GAIN = value;
  this->write_reg_(0x68);
  this->publish_adc_gain();
}

uint8_t RDA5820Component::get_adc_gain() { return (uint8_t) this->state_.ADC_GAIN; }

void RDA5820Component::set_pa_gain(uint8_t value) {
  CHECK_INT_RANGE(value, PA_GAIN_MIN, PA_GAIN_MAX)
  this->state_.PA_GAIN = value;
  this->write_reg_(0x41);
  this->publish_pa_gain();
}

uint8_t RDA5820Component::get_pa_gain() { return (uint8_t) this->state_.PA_GAIN; }

void RDA5820Component::set_pa_ibit(uint8_t value) {
  CHECK_INT_RANGE(value, PA_IBIT_MIN, PA_IBIT_MAX)
  this->state_.PA_IBIT = value;
  this->write_reg_(0x41);
  this->publish_pa_ibit();
}

uint8_t RDA5820Component::get_pa_ibit() { return (uint8_t) this->state_.PA_IBIT; }

void RDA5820Component::set_pa_vcom(uint8_t value) {
  CHECK_INT_RANGE(value, PA_VCOM_MIN, PA_VCOM_MAX)
  this->state_.PA_VCOM = value;
  this->write_reg_(0x41);
  this->publish_pa_vcom();
}

uint8_t RDA5820Component::get_pa_vcom() { return (uint8_t) this->state_.PA_VCOM; }

void RDA5820Component::set_seek_mode(SeekMode value) {
  CHECK_ENUM(value)
  this->state_.SEEK_MODE = (uint16_t) value;
  this->write_reg_(0x05);
  this->publish_seek_mode();
}

SeekMode RDA5820Component::get_seek_mode() { return (SeekMode) this->state_.SEEK_MODE; }

void RDA5820Component::set_seek_th(uint8_t value) {
  CHECK_INT_RANGE(value, SEEK_TH_MIN, SEEK_TH_MAX)
  this->state_.SEEK_TH = value;
  this->write_reg_(0x05);
  this->publish_seek_th();
}

uint8_t RDA5820Component::get_seek_th() { return (uint8_t) this->state_.SEEK_TH; }

void RDA5820Component::set_seek_th_old(uint8_t value) {
  CHECK_INT_RANGE(value, SEEK_TH_OLD_MIN, SEEK_TH_OLD_MAX)
  this->state_.SEEK_TH_OLD = value;
  this->write_reg_(0x07);
  this->publish_seek_th_old();
}

uint8_t RDA5820Component::get_seek_th_old() { return (uint8_t) this->state_.SEEK_TH_OLD; }

void RDA5820Component::set_freq_mode(bool value) {
  this->state_.FREQ_MODE = value ? 1 : 0;
  this->write_reg_(0x07);
  this->publish_freq_mode();
}

bool RDA5820Component::get_freq_mode() { return this->state_.FREQ_MODE == 1; }

void RDA5820Component::set_i2s_enable(bool value) {
  this->state_.I2S_ENABLED = value ? 1 : 0;
  this->write_reg_(0x04);
  this->publish_i2s_enable();
}

bool RDA5820Component::get_i2s_enable() { return this->state_.I2S_ENABLED == 1; }

void RDA5820Component::set_i2s_l_delay(bool value) {
  this->state_.L_DELY = value ? 1 : 0;
  this->write_reg_(0x06);
  this->publish_i2s_l_delay();
}

bool RDA5820Component::get_i2s_l_delay() { return this->state_.L_DELY == 1; }

void RDA5820Component::set_i2s_r_delay(bool value) {
  this->state_.R_DELY = value ? 1 : 0;
  this->write_reg_(0x06);
  this->publish_i2s_r_delay();
}

bool RDA5820Component::get_i2s_r_delay() { return this->state_.R_DELY == 1; }

void RDA5820Component::set_i2s_sclk_o_edge(bool value) {
  this->state_.SCLK_O_EDGE = value ? 1 : 0;
  this->write_reg_(0x06);
  this->publish_i2s_sclk_o_edge();
}

bool RDA5820Component::get_i2s_sclk_o_edge() { return this->state_.SCLK_O_EDGE == 1; }

void RDA5820Component::set_i2s_sw_o_edge(bool value) {
  this->state_.SW_O_EDGE = value ? 1 : 0;
  this->write_reg_(0x06);
  this->publish_i2s_sw_o_edge();
}

bool RDA5820Component::get_i2s_sw_o_edge() { return this->state_.SW_O_EDGE == 1; }

void RDA5820Component::set_i2s_sw_cnt(I2sWordSelect value) {
  CHECK_ENUM(value)
  this->state_.I2S_SW_CNT = (uint16_t) value;
  this->write_reg_(0x06);
  this->publish_i2s_sw_cnt();
}

I2sWordSelect RDA5820Component::get_i2s_sw_cnt() { return (I2sWordSelect) this->state_.I2S_SW_CNT; }

void RDA5820Component::set_i2s_ws_i_edge(bool value) {
  this->state_.WS_I_EDGE = value ? 1 : 0;
  this->write_reg_(0x06);
  this->publish_i2s_ws_i_edge();
}

bool RDA5820Component::get_i2s_ws_i_edge() { return this->state_.WS_I_EDGE == 1; }

void RDA5820Component::set_i2s_data_signed(bool value) {
  this->state_.DATA_SIGNED = value ? 1 : 0;
  this->write_reg_(0x06);
  this->publish_i2s_data_signed();
}

bool RDA5820Component::get_i2s_data_signed() { return this->state_.DATA_SIGNED == 1; }

void RDA5820Component::set_i2s_sclk_i_edge(bool value) {
  this->state_.SCLK_I_EDGE = value ? 1 : 0;
  this->write_reg_(0x06);
  this->publish_i2s_sclk_i_edge();
}

bool RDA5820Component::get_i2s_sclk_i_edge() { return this->state_.SCLK_I_EDGE == 1; }

void RDA5820Component::set_i2s_sw_lr(bool value) {
  this->state_.SW_LR = value ? 1 : 0;
  this->write_reg_(0x06);
  this->publish_i2s_sw_lr();
}

bool RDA5820Component::get_i2s_sw_lr() { return this->state_.SW_LR == 1; }

void RDA5820Component::set_i2s_mode(I2sMode value) {
  CHECK_ENUM(value)
  this->state_.I2S_MODE = (uint16_t) value;
  this->write_reg_(0x06);
  this->publish_i2s_mode();
}

I2sMode RDA5820Component::get_i2s_mode() { return (I2sMode) this->state_.I2S_MODE; }

void RDA5820Component::set_rds_enable(bool value) {
  this->state_.RDS_EN = value ? 1 : 0;
  this->write_reg_(0x02);
  this->publish_rds_enable();
}

bool RDA5820Component::get_rds_enable() { return this->state_.RDS_EN == 1; }

void RDA5820Component::set_rds_deviation(uint8_t value) {
  CHECK_INT_RANGE(value, RDS_DEVIATION_MIN, RDS_DEVIATION_MAX)
  this->state_.RDS_DEV = value;
  this->write_reg_(0x67);
  this->publish_rds_deviation();
}

uint8_t RDA5820Component::get_rds_deviation() { return (uint8_t) this->state_.RDS_DEV; }

void RDA5820Component::set_rds_station(const std::string &value) {
  this->rds_station_ = value;
  CHECK_TEXT_RANGE(this->rds_station_, RDS_STATION_MAX)
  this->publish_rds_station();
}

std::string RDA5820Component::get_rds_station() { return this->rds_station_; }

void RDA5820Component::set_rds_text(const std::string &value) {
  this->rds_text_ = value;
  CHECK_TEXT_RANGE(this->rds_text_, RDS_TEXT_MAX)
  this->publish_rds_text();
}

std::string RDA5820Component::get_rds_text() { return this->rds_text_; }

void RDA5820Component::set_gpio1(Gpio1Control value) {
  CHECK_ENUM(value)
  this->state_.GPIO1 = (uint16_t) value;
  this->write_reg_(0x04);
  this->publish_gpio1();
}

Gpio1Control RDA5820Component::get_gpio1() { return (Gpio1Control) this->state_.GPIO1; }

void RDA5820Component::set_gpio2(Gpio2Control value) {
  CHECK_ENUM(value)
  this->state_.GPIO2 = (uint16_t) value;
  this->write_reg_(0x04);
  this->publish_gpio2();
}

Gpio2Control RDA5820Component::get_gpio2() { return (Gpio2Control) this->state_.GPIO2; }

void RDA5820Component::set_gpio3(Gpio3Control value) {
  CHECK_ENUM(value)
  this->state_.GPIO3 = (uint16_t) value;
  this->write_reg_(0x04);
  this->publish_gpio3();
}

Gpio3Control RDA5820Component::get_gpio3() { return (Gpio3Control) this->state_.GPIO3; }

std::string RDA5820Component::get_chip_id_text_sensor() { return this->chip_id_; }

float RDA5820Component::get_frequency_sensor() {
  uint16_t band = BAND[this->state_.BAND];
  uint16_t space = SPACE[this->state_.SPACE];
  if ((BandSelect) this->state_.BAND == BandSelect::BAND_65_76_OR_50_65 && this->state_.MODE_50M_65M == 1) {
    band = 50;
  }
  return (float) this->state_.READCHAN / space - band;
}

float RDA5820Component::get_rssi_sensor() { return (float) this->state_.RSSI; }

bool RDA5820Component::get_stereo_binary_sensor() { return this->state_.ST == 1; }

bool RDA5820Component::get_seek_fail_binary_sensor() { return this->state_.SF == 1; }

bool RDA5820Component::get_seek_tune_complete_binary_sensor() { return this->state_.STC == 1; }

bool RDA5820Component::get_fm_ready_binary_sensor() { return this->state_.FM_READY == 1; }

bool RDA5820Component::get_fm_is_station_binary_sensor() { return this->state_.FM_TRUE == 1; }

bool RDA5820Component::get_blk_e_found_binary_sensor() { return this->state_.BLK_E == 1; }

bool RDA5820Component::get_rds_decoder_sync_binary_sensor() { return this->state_.RDSS == 1; }

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
