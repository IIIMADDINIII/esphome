import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import i2c, sensor, text_sensor
from esphome.const import (
    CONF_ID,
    CONF_SENSOR,
    CONF_FREQUENCY,
    ICON_CHIP,
    ENTITY_CATEGORY_DIAGNOSTIC,
)

CODEOWNERS = ["@gabest11"]
DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["sensor", "text_sensor", "number", "switch", "select", "text"]
MULTI_CONF = True

UNIT_MEGA_HERTZ = "MHz"
UNIT_KILO_HERTZ = "kHz"
UNIT_MILLI_VOLT = "mV"
UNIT_MICRO_AMPERE = "mA"
UNIT_DECIBEL_MICRO_VOLT = "dBuV"

ICON_VOLUME_MUTE = "mdi:volume-mute"
ICON_EAR_HEARING = "mdi:ear-hearing"
ICON_RADIO_TOWER = "mdi:radio-tower"
ICON_SLEEP = "mdi:sleep"
ICON_SINE_WAVE = "mdi:sine-wave"
ICON_RESISTOR = "mdi:resistor"
ICON_FORMAT_TEXT = "mdi:format-text"

rda5820_ns = cg.esphome_ns.namespace("rda5820")
RDA5820Component = rda5820_ns.class_(
    "RDA5820Component", cg.PollingComponent, i2c.I2CDevice
)

CONF_RDA5820_ID = "rda5820_id"
# general config
# CONF_FREQUENCY = "frequency"
# sensor
CONF_CHIP_ID = "chip_id"

SetFrequencyAction = rda5820_ns.class_(
    "SetFrequencyAction", automation.Action, cg.Parented.template(RDA5820Component)
)

SENSOR_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_CHIP_ID): text_sensor.text_sensor_schema(
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            icon=ICON_CHIP,
        ),
    }
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(RDA5820Component),
            cv.Optional(CONF_FREQUENCY, default=87.50): cv.float_range(76, 108),
            cv.Optional(CONF_SENSOR): SENSOR_SCHEMA,
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x2C))
)


VARIABLES = {
    None: [
        [CONF_FREQUENCY],
    ],
}

SENSORS = {
    CONF_SENSOR: [
        [CONF_CHIP_ID, "text_sensor"],
    ]
}


async def for_each_conf(config, vars, callback):
    for section in vars:
        c = config[section] if section in config else config
        for args in vars[section]:
            setter = "set_"
            if section is not None and section != CONF_SENSOR:
                setter += section + "_"
            setter += args[0]
            if cc := c.get(args[0]):
                await callback(cc, args, setter)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    async def set_var(c, a, s):
        cg.add(getattr(var, s)(c))

    await for_each_conf(config, VARIABLES, set_var)

    async def new_sensor(c, args, setter):
        s = None
        if args[1] == "sensor":
            s = await sensor.new_sensor(c)
        elif args[1] == "text_sensor":
            s = await text_sensor.new_text_sensor(c)
        cg.add(getattr(var, setter + "_" + args[1])(s))

    await for_each_conf(config, SENSORS, new_sensor)


FREQUENCY_SCHEMA = automation.maybe_simple_id(
    {
        cv.GenerateID(): cv.use_id(RDA5820Component),
        cv.Required(CONF_FREQUENCY): cv.float_range(min=76, max=108),
    }
)


@automation.register_action(
    "rda5820.set_frequency", SetFrequencyAction, FREQUENCY_SCHEMA
)
async def tune_frequency_action_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])
    if frequency := config.get(CONF_FREQUENCY):
        template_ = await cg.templatable(frequency, args, cg.float_)
        cg.add(var.set_frequency(template_))
    return var
