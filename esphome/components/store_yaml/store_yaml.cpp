#include "store_yaml.h"
#include "esphome/core/log.h"
#include <string>
#include <map>

namespace esphome {
namespace store_yaml {

static const char *const TAG = "store_yaml";

void StoreYamlComponent::dump_config() {
  if (this->show_in_dump_config_) {
    ESP_LOGCONFIG(TAG, "YAML:");
    this->log(true);
  }
}

class Decompressor {
  std::map<uint16_t, std::string> codes_;  // TODO: replace string with char + index to next char
  size_t pos_{0};
  uint8_t size_{0};
  uint32_t buff_{0};
  size_t code_index_;
  uint8_t code_width_;

 public:
  Decompressor() {
    for (int i = 0; i < 256; i++) {
      this->codes_[i] = std::string(1, (char) i);
    }
    this->code_index_ = this->codes_.size();
    this->code_width_ = 9;  // log2next + 1
  }

  uint32_t get_bits(size_t bits) {
    if (this->is_eof())
      return UINT32_MAX;

    while (this->size_ < bits) {
      this->buff_ = (this->buff_ << 8) | ESPHOME_YAML[this->pos_++];
      this->size_ += 8;
    }

    uint32_t value = (this->buff_ >> (this->size_ - bits)) & ((1 << bits) - 1);
    this->size_ -= bits;
    this->buff_ &= (1 << this->size_) - 1;
    return value;
  }

  std::string get_code() {
    if (this->code_index_ == ((1 << this->code_width_) - 1)) {
      this->code_width_++;
    }
    uint32_t c = get_bits(this->code_width_);
    auto i = this->codes_.find(c);
    if (i != this->codes_.end()) {
      return i->second;
    }
    if (c != this->codes_.size()) {
      this->pos_ = ESPHOME_YAML_SIZE;  // error in input, set eof
    }
    return std::string();
  }

  void add_code(const std::string &c) { this->codes_[this->code_index_++] = c; }

  bool is_eof() const { return this->pos_ >= ESPHOME_YAML_SIZE; }
};

void StoreYamlComponent::log(bool dump_config) const {
  Decompressor *dec = new Decompressor();

  std::string c = dec->get_code();
  std::string prev = c;
  std::string yaml = c;

  while (!dec->is_eof()) {
    std::string c = dec->get_code();
    if (c.empty())
      c = prev + prev[0];
    dec->add_code(prev + c[0]);
    prev = c;
    yaml += c;
    // print line by line because the logger cannot handle too much data
    // it also uses less memory
    size_t newline = 0;
    while (newline != std::string::npos) {
      newline = yaml.find('\n');
      if (newline != std::string::npos) {
        std::string row = yaml.substr(0, newline);
        yaml = std::string(yaml.begin() + newline + 1, yaml.end());
        if (dump_config) {
          ESP_LOGCONFIG(TAG, "%s", row.c_str());
        } else {
          ESP_LOGI(TAG, "%s", row.c_str());
        }
      }
    }
  }

  delete dec;
}

}  // namespace store_yaml
}  // namespace esphome
