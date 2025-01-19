#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace mcp3421 {

enum MCP3421Gain {
  MCP3421_GAIN_1 = 0b00,
  MCP3421_GAIN_2 = 0b01,
  MCP3421_GAIN_4 = 0b10,
  MCP3421_GAIN_8 = 0b11,
};

enum MCP3421Resolution {
  MCP3421_12_BITS = 0b00,
  MCP3421_14_BITS = 0b01,
  MCP3421_16_BITS = 0b10,
  MCP3421_18_BITS = 0b11,
};

typedef union {
  struct {
    uint8_t GAIN : 2;
    uint8_t SR   : 2;
    uint8_t OC   : 1;
    uint8_t Cx   : 2;
    uint8_t RDY  : 1;
  } bit;
} MCP3421Config;

static const uint8_t MCP3421_DEFAULT_ADDR = 0x68;

class MCP3421Sensor : public sensor::Sensor, public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;
  void update() override;
  void set_gain(MCP3421Gain gain) { this->gain_ = gain; }
  void set_resolution(MCP3421Resolution resolution) { this->resolution_ = resolution; }
 private:
  MCP3421Gain gain_;
  MCP3421Resolution resolution_;
};

}  // namespace mcp3421
}  // namespace esphome
