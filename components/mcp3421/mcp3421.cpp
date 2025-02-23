#include "mcp3421.h"
#include "esphome/core/log.h"

namespace esphome {
namespace mcp3421 {

static const char *TAG = "mcp3421";

void MCP3421Sensor::setup() {
  ESP_LOGI(TAG, "Initializing MCP3421...");
  Wire.begin();
}

void MCP3421Sensor::dump_config() {
  ESP_LOGCONFIG(TAG, "MCP3421:");
  ESP_LOGCONFIG(TAG, "  Gain: %d", this->gain_);
  ESP_LOGCONFIG(TAG, "  Resolution: %d", this->resolution_);
}

float MCP3421Sensor::get_setup_priority() const {
  return esphome::setup_priority::DATA;
}

void MCP3421Sensor::update() {
  Wire.requestFrom(MCP3421_DEFAULT_ADDR, 3);  // Read 3 bytes from MCP3421
  if (Wire.available() == 3) {
    uint8_t msb = Wire.read();
    uint8_t lsb = Wire.read();
    uint8_t config = Wire.read();

    int32_t value = (msb << 8) | lsb;
    if (value & 0x8000) value -= 65536;  // Convert to signed integer

    float voltage = value * 0.000015625;  // Convert ADC value to voltage
    ESP_LOGD(TAG, "Raw ADC: %d, Voltage: %.6f V", value, voltage);
    publish_state(voltage);
  } else {
    ESP_LOGE(TAG, "Failed to read from MCP3421");
  }
}

}  // namespace mcp3421
}  // namespace esphome
