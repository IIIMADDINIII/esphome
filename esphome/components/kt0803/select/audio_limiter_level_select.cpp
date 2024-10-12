#include "audio_limiter_level_select.h"

namespace esphome {
namespace kt0803 {

void AudioLimiterLevelSelect::control(const std::string &value) {
  this->publish_state(value);
  if (auto index = this->active_index()) {
    this->parent_->set_audio_limiter_level((AudioLimiterLevel) *index);
  }
}

}  // namespace kt0803
}  // namespace esphome
