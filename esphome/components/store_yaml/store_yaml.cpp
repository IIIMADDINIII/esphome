#include "store_yaml.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace store_yaml {

static const char *const TAG = "store_yaml";

void StoreYamlComponent::dump_config() {
  if (this->show_in_dump_config_) {
    ESP_LOGCONFIG(TAG, "YAML:");
    RowDecompressor dec(ESPHOME_YAML, ESPHOME_YAML_SIZE);
    std::string row;
    while (dec.get_row(row)) {
      ESP_LOGCONFIG(TAG, "%s", row.c_str());
    }
  }
}

void StoreYamlComponent::loop() {
  if (this->dec_) {
    std::string row;
    if (this->dec_->get_row(row)) {
      ESP_LOGI(TAG, "%s", row.c_str());
    } else {
      this->dec_ = nullptr;
    }
  }
}

void StoreYamlComponent::log() {
  ESP_LOGI(TAG, "YAML:");
  this->dec_ = make_unique<RowDecompressor>(ESPHOME_YAML, ESPHOME_YAML_SIZE);
}

}  // namespace store_yaml
}  // namespace esphome
