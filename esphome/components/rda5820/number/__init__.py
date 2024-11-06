import esphome.codegen as cg
from esphome.components import number
from esphome.components.number import NumberMode
import esphome.config_validation as cv
from esphome.const import (
    CONF_FREQUENCY,
    CONF_MODE,
    UNIT_PERCENT,
    UNIT_EMPTY,
    DEVICE_CLASS_FREQUENCY,
    DEVICE_CLASS_EMPTY,
    ENTITY_CATEGORY_CONFIG,
)
from .. import (
    CONF_RDA5820_ID,
    RDA5820Component,
    rda5820_ns,
    CONF_RDS,
    CONF_VOLUME,
    CONF_TH_SOFTBLEND,
    CONF_AUDIO_DEV,
    CONF_PILOT_DEV,
    CONF_PGA_GAIN,
    CONF_ADC_GAIN,
    CONF_PA_GAIN,
    CONF_PA_IBIT,
    CONF_PA_VCOM,
    CONF_SEEK_TH,
    CONF_SEEK_TH_OLD,
    CONF_DEVIATION,
    UNIT_MEGA_HERTZ,
    # UNIT_,
    for_each_conf,
)

FrequencyNumber = rda5820_ns.class_("FrequencyNumber", number.Number)
VolumeNumber = rda5820_ns.class_("VolumeNumber", number.Number)
ThSoftblendNumber = rda5820_ns.class_("ThSoftblendNumber", number.Number)
AudioDevNumber = rda5820_ns.class_("AudioDevNumber", number.Number)
PilotDevNumber = rda5820_ns.class_("PilotDevNumber", number.Number)
PgaGainNumber = rda5820_ns.class_("PgaGainNumber", number.Number)
AdcGainNumber = rda5820_ns.class_("AdcGainNumber", number.Number)
PaGainNumber = rda5820_ns.class_("PaGainNumber", number.Number)
PaIbitNumber = rda5820_ns.class_("PaIbitNumber", number.Number)
PaVcomNumber = rda5820_ns.class_("PaVcomNumber", number.Number)
SeekThNumber = rda5820_ns.class_("SeekThNumber", number.Number)
SeekThOldNumber = rda5820_ns.class_("SeekThOldNumber", number.Number)
RdsDeviationNumber = rda5820_ns.class_("RdsDeviationNumber", number.Number)

RDS_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_DEVIATION): number.number_schema(
            RdsDeviationNumber,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
    }
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_RDA5820_ID): cv.use_id(RDA5820Component),
        cv.Optional(CONF_FREQUENCY): number.number_schema(
            FrequencyNumber,
            unit_of_measurement=UNIT_MEGA_HERTZ,
            device_class=DEVICE_CLASS_FREQUENCY,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_VOLUME): number.number_schema(
            VolumeNumber,
            unit_of_measurement=UNIT_PERCENT,
            # device_class=DEVICE_CLASS_,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_TH_SOFTBLEND): number.number_schema(
            ThSoftblendNumber,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_AUDIO_DEV): number.number_schema(
            AudioDevNumber,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_PILOT_DEV): number.number_schema(
            PilotDevNumber,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_PGA_GAIN): number.number_schema(
            PgaGainNumber,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_ADC_GAIN): number.number_schema(
            AdcGainNumber,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_PA_GAIN): number.number_schema(
            PaGainNumber,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_PA_IBIT): number.number_schema(
            PaIbitNumber,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_PA_VCOM): number.number_schema(
            PaVcomNumber,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_SEEK_TH): number.number_schema(
            SeekThNumber,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_SEEK_TH_OLD): number.number_schema(
            SeekThOldNumber,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_RDS): RDS_SCHEMA,
    }
)

VARIABLES = {
    None: [
        [CONF_FREQUENCY, 50, 108, 0.025, None],
        [CONF_VOLUME, 0, 100, 1, None],
        [CONF_TH_SOFTBLEND, 0, 30, 1, None],
        [CONF_AUDIO_DEV, 0, 255, 1, None],
        [CONF_PILOT_DEV, 0, 255, 1, None],
        [CONF_PGA_GAIN, 0, 7, 1, None],
        [CONF_ADC_GAIN, 0, 7, 1, None],
        [CONF_PA_GAIN, 0, 63, 1, None],
        [CONF_PA_IBIT, 0, 7, 1, None],
        [CONF_PA_VCOM, 0, 7, 1, None],
        [CONF_SEEK_TH, 0, 15, 1, None],
        [CONF_SEEK_TH_OLD, 0, 63, 1, None],
    ],
    CONF_RDS: [
        [CONF_DEVIATION, 0, 255, 1, None],
    ],
}


async def to_code(config):
    parent = await cg.get_variable(config[CONF_RDA5820_ID])

    async def new_number(c, args, setter):
        # only override mode when it's set to auto in user config
        if CONF_MODE not in c or c[CONF_MODE] == number.NumberMode.NUMBER_MODE_AUTO:
            if args[4] is not None:
                c[CONF_MODE] = args[4]
        n = await number.new_number(
            c, min_value=args[1], max_value=args[2], step=args[3]
        )
        await cg.register_parented(n, parent)
        cg.add(getattr(parent, setter + "_number")(n))

    await for_each_conf(config, VARIABLES, new_number)
