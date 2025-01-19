import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_GAIN,
    CONF_RESOLUTION,
    DEVICE_CLASS_VOLTAGE,
    STATE_CLASS_MEASUREMENT,
    UNIT_VOLT,
)

CODEOWNERS = ["EDV64"]
DEPENDENCIES = ["i2c"]

mcp3421_ns = cg.esphome_ns.namespace("mcp3421")

MCP3421Gain = mcp3421_ns.enum("MCP3421Gain")
GAIN = {
    1: MCP3421Gain.MCP3421_GAIN_1,
    2: MCP3421Gain.MCP3421_GAIN_2,
    4: MCP3421Gain.MCP3421_GAIN_4,
    8: MCP3421Gain.MCP3421_GAIN_8,
}

MCP3421Resolution = mcp3421_ns.enum("MCP3421Resolution")
RESOLUTION = {
    12: MCP3421Resolution.MCP3421_12_BITS,
    14: MCP3421Resolution.MCP3421_14_BITS,
    16: MCP3421Resolution.MCP3421_16_BITS,
    18: MCP3421Resolution.MCP3421_18_BITS,
}

MCP3421Sensor = mcp3421_ns.class_(
    "MCP3421Sensor", sensor.Sensor, cg.PollingComponent, i2c.I2CDevice
)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        MCP3421Sensor,
        unit_of_measurement=UNIT_VOLT,
        accuracy_decimals=6,
        device_class=DEVICE_CLASS_VOLTAGE,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(
        {
            cv.Optional(CONF_GAIN, default=1): cv.enum(GAIN, int=True),
            cv.Optional(CONF_RESOLUTION, default=18): cv.enum(RESOLUTION, int=True),
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x68))
)

async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    cg.add(var.set_gain(config[CONF_GAIN]))
    cg.add(var.set_resolution(config[CONF_RESOLUTION]))