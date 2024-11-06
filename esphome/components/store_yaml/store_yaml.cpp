#include "store_yaml.h"
#include "esphome/core/log.h"

namespace esphome {
namespace store_yaml {

static const char *const TAG = "store_yaml";

void StoreYamlComponent::dump_config() {
  if (this->show_) {
    ESP_LOGCONFIG(TAG, "YAML:");
    this->log(true);
  }
}

std::string StoreYamlComponent::get_yaml() const { return this->yaml_; }

void StoreYamlComponent::log(bool dump_config) const {
  const char *s = this->yaml_.c_str();
  while (*s) {
    const char *e = s;
    while (*e && *e++ != '\n')
      ;
    const char *tmp = e;
    if (e > s) {
      if (e[-1] == '\n')
        e--;
      std::string row = std::string(s, e - s);
      if (dump_config) {
        ESP_LOGCONFIG(TAG, "%s", row.c_str());
      } else {
        ESP_LOGI(TAG, "%s", row.c_str());
      }
    }
    s = tmp;
  }
}

}  // namespace store_yaml
}  // namespace esphome
