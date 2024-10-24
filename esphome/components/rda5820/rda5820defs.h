#pragma once

namespace esphome {
namespace rda5820 {

static const uint8_t REG_ = 0x00;

static const float FREQ_MIN = 76;   // TODO
static const float FREQ_MAX = 108;  // TODO

struct RDA5820State {
  union {
    uint8_t REG_;
    struct {
      uint8_t _ : 8;
    };
  };
};

}  // namespace rda5820
}  // namespace esphome
