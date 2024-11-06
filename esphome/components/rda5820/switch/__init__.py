import esphome.codegen as cg
from esphome.components import switch
import esphome.config_validation as cv
from esphome.const import (
    DEVICE_CLASS_SWITCH,
    ENTITY_CATEGORY_CONFIG,
)
from .. import (
    CONF_RDA5820_ID,
    RDA5820Component,
    rda5820_ns,
    CONF_I2S,
    CONF_RDS,
    CONF_ENABLE,
    CONF_MONO,
    CONF_MUTE,
    CONF_SOFTMUTE,
    CONF_BASS_BOOST,
    CONF_AUDIO_HI_Z,
    CONF_POWER_ENABLE,
    CONF_AFC_ENABLE,
    CONF_RBDS_ENABLE,
    CONF_SOFTBLEND_ENABLE,
    CONF_RCLK_DIRECT_INPUT_MODE,
    CONF_RCLK_NON_CALIBRATE_MODE,
    CONF_L_DELAY,
    CONF_R_DELAY,
    CONF_SCLK_O_EDGE,
    CONF_SW_O_EDGE,
    CONF_WS_I_EDGE,
    CONF_DATA_SIGNED,
    CONF_SCLK_I_EDGE,
    CONF_SW_LR,
    CONF_FREQ_MODE,
    for_each_conf,
)

MonoSwitch = rda5820_ns.class_("MonoSwitch", switch.Switch)
MuteSwitch = rda5820_ns.class_("MuteSwitch", switch.Switch)
SoftmuteSwitch = rda5820_ns.class_("SoftmuteSwitch", switch.Switch)
BassBoostSwitch = rda5820_ns.class_("BassBoostSwitch", switch.Switch)
AudioHiZSwitch = rda5820_ns.class_("AudioHiZSwitch", switch.Switch)
PowerEnableSwitch = rda5820_ns.class_("PowerEnableSwitch", switch.Switch)
AfcEnableSwitch = rda5820_ns.class_("AfcEnableSwitch", switch.Switch)
RbdsEnableSwitch = rda5820_ns.class_("RbdsEnableSwitch", switch.Switch)
SoftblendEnableSwitch = rda5820_ns.class_("SoftblendEnableSwitch", switch.Switch)
RclkDirectInputModeSwitch = rda5820_ns.class_(
    "RclkDirectInputModeSwitch", switch.Switch
)
RclkNonCalibrateModeSwitch = rda5820_ns.class_(
    "RclkNonCalibrateModeSwitch", switch.Switch
)
I2sEnableSwitch = rda5820_ns.class_("I2sEnableSwitch", switch.Switch)
I2sLDelaySwitch = rda5820_ns.class_("I2sLDelaySwitch", switch.Switch)
I2sRDelaySwitch = rda5820_ns.class_("I2sRDelaySwitch", switch.Switch)
I2sSclkOEdgeSwitch = rda5820_ns.class_("I2sSclkOEdgeSwitch", switch.Switch)
I2sSwOEdgeSwitch = rda5820_ns.class_("I2sSwOEdgeSwitch", switch.Switch)
I2sWsIEdgeSwitch = rda5820_ns.class_("I2sWsIEdgeSwitch", switch.Switch)
I2sDataSignedSwitch = rda5820_ns.class_("I2sDataSignedSwitch", switch.Switch)
I2sSclkIEdgeSwitch = rda5820_ns.class_("I2sSclkIEdgeSwitch", switch.Switch)
I2sSwLrSwitch = rda5820_ns.class_("I2sSwLrSwitch", switch.Switch)
FreqModeSwitch = rda5820_ns.class_("FreqModeSwitch", switch.Switch)
RdsEnableSwitch = rda5820_ns.class_("RdsEnableSwitch", switch.Switch)

I2S_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_ENABLE): switch.switch_schema(
            I2sEnableSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_L_DELAY): switch.switch_schema(
            I2sLDelaySwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_R_DELAY): switch.switch_schema(
            I2sRDelaySwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_SCLK_O_EDGE): switch.switch_schema(
            I2sSclkOEdgeSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_SW_O_EDGE): switch.switch_schema(
            I2sSwOEdgeSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_WS_I_EDGE): switch.switch_schema(
            I2sWsIEdgeSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_DATA_SIGNED): switch.switch_schema(
            I2sDataSignedSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_SCLK_I_EDGE): switch.switch_schema(
            I2sSclkIEdgeSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_SW_LR): switch.switch_schema(
            I2sSwLrSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
    }
)

RDS_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_ENABLE): switch.switch_schema(
            RdsEnableSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
    }
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_RDA5820_ID): cv.use_id(RDA5820Component),
        cv.Optional(CONF_MONO): switch.switch_schema(
            MonoSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_MUTE): switch.switch_schema(
            MuteSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_SOFTMUTE): switch.switch_schema(
            SoftmuteSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_BASS_BOOST): switch.switch_schema(
            BassBoostSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_AUDIO_HI_Z): switch.switch_schema(
            AudioHiZSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_POWER_ENABLE): switch.switch_schema(
            PowerEnableSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_AFC_ENABLE): switch.switch_schema(
            AfcEnableSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_RBDS_ENABLE): switch.switch_schema(
            RbdsEnableSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_SOFTBLEND_ENABLE): switch.switch_schema(
            SoftblendEnableSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_RCLK_DIRECT_INPUT_MODE): switch.switch_schema(
            RclkDirectInputModeSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_RCLK_NON_CALIBRATE_MODE): switch.switch_schema(
            RclkNonCalibrateModeSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_FREQ_MODE): switch.switch_schema(
            FreqModeSwitch,
            device_class=DEVICE_CLASS_SWITCH,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_I2S): I2S_SCHEMA,
        cv.Optional(CONF_RDS): RDS_SCHEMA,
    }
)

VARIABLES = {
    None: [
        [CONF_MONO],
        [CONF_MUTE],
        [CONF_SOFTMUTE],
        [CONF_BASS_BOOST],
        [CONF_AUDIO_HI_Z],
        [CONF_POWER_ENABLE],
        [CONF_AFC_ENABLE],
        [CONF_RBDS_ENABLE],
        [CONF_SOFTBLEND_ENABLE],
        [CONF_RCLK_DIRECT_INPUT_MODE],
        [CONF_RCLK_NON_CALIBRATE_MODE],
        [CONF_ENABLE],
        [CONF_L_DELAY],
        [CONF_R_DELAY],
        [CONF_SCLK_O_EDGE],
        [CONF_SW_O_EDGE],
        [CONF_WS_I_EDGE],
        [CONF_DATA_SIGNED],
        [CONF_SCLK_I_EDGE],
        [CONF_SW_LR],
        [CONF_FREQ_MODE],
    ],
    CONF_RDS: [
        [CONF_ENABLE],
    ],
}


async def to_code(config):
    parent = await cg.get_variable(config[CONF_RDA5820_ID])

    async def new_switch(c, args, setter):
        s = await switch.new_switch(c)
        await cg.register_parented(s, parent)
        cg.add(getattr(parent, setter + "_switch")(s))
        # if args[0] == CONF_GPIO1:
        # cg.add(s.set_pin(1))
        # elif args[0] == CONF_GPIO2:
        # cg.add(s.set_pin(2))
        # elif args[0] == CONF_GPIO3:
        # cg.add(s.set_pin(3))

    await for_each_conf(config, VARIABLES, new_switch)
