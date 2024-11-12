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

void StoreYamlComponent::setup() {
  if (this->web_server_ != nullptr) {
    this->web_server_->init();
    this->web_server_->add_handler(this);
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

void StoreYamlComponent::set_web_server(web_server_base::WebServerBase *web_server, const std::string &url) {
  this->web_server_ = web_server;
  this->web_server_url_ = url;
}

bool StoreYamlComponent::canHandle(AsyncWebServerRequest *request) {
  return request->method() == HTTP_GET && request->url() == this->web_server_url_.c_str();
}

void StoreYamlComponent::handleRequest(AsyncWebServerRequest *request) {
#ifdef USE_ARDUINO
  auto cb = [this](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
    uint8_t *ptr = buffer;
    // 5KB+ config file with a single character repeating will result in a 100 byte long word, not likely
    while (maxLen > 100 && !(this->web_dec_ && this->web_dec_->is_eof())) {
      std::string s;
      if (!this->web_dec_) {
        this->web_dec_ = make_unique<Decompressor>(ESPHOME_YAML, ESPHOME_YAML_SIZE);
        s = this->web_dec_->get_first();
      } else {
        s = this->web_dec_->get_next();
      }
      size_t len = std::min(maxLen, s.size());
      memcpy(ptr, s.c_str(), len);
      ptr += len;
      maxLen -= len;
    }
    return ptr - buffer;
  };
  AsyncWebServerResponse *response = request->beginChunkedResponse("text/plain;charset=UTF-8", cb);
#else
  AsyncResponseStream *response = request->beginResponseStream("text/plain;charset=UTF-8");
  auto dec = make_unique<Decompressor>(ESPHOME_YAML, ESPHOME_YAML_SIZE);
  response->print(dec->get_first().c_str());
  while (!dec->is_eof()) {
    response->print(dec->get_next().c_str());
  }
  dec = nullptr;
#endif

  request->send(response);
}

}  // namespace store_yaml
}  // namespace esphome
