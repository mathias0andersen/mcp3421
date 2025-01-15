#include "mcp3421.h"
#include "esphome/core/log.h"

namespace esphome {
namespace mcp3421 {

static const char *const TAG = "mcp3421";

void MCP3421Sensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up %s...", this->name_.c_str());

/*  uint16_t manu = 0;
  if (!this->read_byte_16(MCP9808_REG_MANUF_ID, &manu) || manu != MCP9808_MANUF_ID) {
    this->mark_failed();
    ESP_LOGE(TAG, "%s manufacuturer id failed, device returned %X", this->name_.c_str(), manu);
    return;
  }
  uint16_t dev_id = 0;
  if (!this->read_byte_16(MCP9808_REG_DEVICE_ID, &dev_id) || dev_id != MCP9808_DEV_ID) {
    this->mark_failed();
    ESP_LOGE(TAG, "%s device id failed, device returned %X", this->name_.c_str(), dev_id);
    return;
  }
*/
}

void MCP3421Sensor::dump_config() {
  ESP_LOGCONFIG(TAG, "%s:", this->name_.c_str());
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with %s failed!", this->name_.c_str());
  }
  LOG_UPDATE_INTERVAL(this);
//  LOG_SENSOR("  ", "Temperature", this);
}

void MCP9808Sensor::update() {
  uint16_t raw_temp;
  if (!this->read_byte_16(MCP9808_REG_AMBIENT_TEMP, &raw_temp)) {
    this->status_set_warning();
    return;
  }
  if (raw_temp == 0xFFFF) {
    this->status_set_warning();
    return;
  }

  float temp = NAN;
  uint8_t msb = (uint8_t) ((raw_temp & 0xff00) >> 8);
  uint8_t lsb = raw_temp & 0x00ff;

  msb = msb & MCP9808_AMBIENT_CLEAR_FLAGS;

  if ((msb & MCP9808_AMBIENT_TEMP_NEGATIVE) == MCP9808_AMBIENT_TEMP_NEGATIVE) {
    msb = msb & MCP9808_AMBIENT_CLEAR_SIGN;
    temp = (256 - ((uint16_t) (msb) *16 + lsb / 16.0f)) * -1;
  } else {
    temp = (uint16_t) (msb) *16 + lsb / 16.0f;
  }

  if (std::isnan(temp)) {
    this->status_set_warning();
    return;
  }

  ESP_LOGD(TAG, "%s: Got temperature=%.4f°C", this->name_.c_str(), temp);
  this->publish_state(temp);
  this->status_clear_warning();
}

float MCP3421Sensor::get_setup_priority() const { return setup_priority::DATA; }

}  // namespace mcp9808
}  // namespace esphome
