#pragma once

#include <stdint.h>

namespace esphome {
namespace hlw8032 {

struct register_set {
  union {
    uint8_t state; // AA: fail, 55: ok, F?: overflow
    struct {
      uint8_t error : 1;
      uint8_t p_overflow : 1;
      uint8_t c_overflow : 1;
      uint8_t v_overflow : 1;
      uint8_t : 4; // F
    };
  };
  union {
    uint32_t vars[6];
    struct {
      uint32_t vp, v;
      uint32_t cp, c;
      uint32_t pp, p;
    };
  };
  union {
    uint8_t upd;
    struct {
      uint8_t : 4;
      uint8_t p_valid : 1;
      uint8_t c_valid : 1;
      uint8_t v_valid : 1;
      uint8_t pf_carry : 1;
    };
  };
  uint16_t pf;
};

}  // namespace hlw8032
}  // namespace esphome
