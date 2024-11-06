#include "store_yaml.h"
#include "esphome/core/log.h"

namespace esphome {
namespace store_yaml {

static const char *const TAG = "store_yaml";

void StoreYamlComponent::set(const std::string &yaml) { this->yaml_ = yaml; }

std::string StoreYamlComponent::get() const { return this->yaml_; }

void StoreYamlComponent::dump() const {
  const char *s = this->yaml_.c_str();
  while (*s) {
    const char *e = s;
    while (*e && *e++ != '\n')
      ;
    const char *tmp = e;
    if (e > s) {
      if (e[-1] == '\n')
        e--;
      ESP_LOGI(TAG, "%s", std::string(s, e - s).c_str());
    }
    s = tmp;
  }
}

}  // namespace store_yaml
}  // namespace esphome
