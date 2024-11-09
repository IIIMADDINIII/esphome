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

struct Entry {
  uint32_t p : 24;
  uint32_t c : 8;
};

class Decompressor {
  std::map<uint16_t, Entry> codes_;
  size_t pos_;
  uint8_t size_;
  uint32_t buff_;
  size_t code_index_;
  uint8_t code_width_;
  Entry prev_;

 public:
  Decompressor() { this->reset(); }

  void reset() {
    this->pos_ = 0;
    this->size_ = 0;
    this->buff_ = 0;
    this->codes_.clear();
    for (uint32_t i = 0; i < 256; i++) {
      this->codes_[i] = Entry{.p = 0, .c = i};
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

  const Entry *get_entry(uint16_t &code) {
    if (this->code_index_ == ((1 << this->code_width_) - 1)) {
      this->code_width_++;
    }
    code = (uint16_t) get_bits(this->code_width_);
    auto i = this->codes_.find(code);
    if (i != this->codes_.end()) {
      return &i->second;
    }
    if (code != this->codes_.size()) {
      this->pos_ = ESPHOME_YAML_SIZE;  // error in input, set eof
    }
    return nullptr;
  }

  bool is_eof() const { return this->pos_ >= ESPHOME_YAML_SIZE; }

  std::string get_string(const Entry *entry) const {
    std::string s(1, (char) entry->c);
    while (entry->p != 0) {
      entry = &this->codes_.find(entry->p)->second;
      s = std::string(1, (char) entry->c) + s;
    }
    return s;
  }

  std::string get_first() {
    this->reset();
    uint16_t code = 0;
    const Entry *entry = this->get_entry(code);
    std::string s = this->get_string(entry);
    this->prev_.c = (uint32_t) s[0];
    this->prev_.p = code;
    return s;
  }

  std::string get_next() {
    uint16_t code = 0;
    const Entry *entry = this->get_entry(code);
    if (entry == nullptr)
      entry = &this->prev_;
    std::string s = this->get_string(entry);
    this->prev_.c = s[0];
    this->codes_[this->code_index_++] = this->prev_;
    this->prev_.p = code;
    return s;
  }
};

void StoreYamlComponent::log(bool dump_config) const {
  Decompressor *dec = new Decompressor();
  std::string yaml = dec->get_first();
  while (!dec->is_eof()) {
    yaml += dec->get_next();
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
