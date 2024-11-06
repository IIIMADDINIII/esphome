import esphome.codegen as cg
from esphome.components import select
import esphome.config_validation as cv
from esphome.const import (
    CONF_MODE,
    ENTITY_CATEGORY_CONFIG,
)
from .. import (
    CONF_RDA5820_ID,
    RDA5820Component,
    rda5820_ns,
    CONF_WORK_MODE,
    CONF_LNA_PORT_SEL,
    CONF_DE_EMPHASIS,
    CONF_CLK_MODE,
    CONF_SW_CNT,
    CONF_GPIO1,
    CONF_GPIO2,
    CONF_GPIO3,
    CONF_SEEK_MODE,
    WORK_MODE,
    LNA_INPUT_PORT,
    DE_EMPHASIS,
    CLK_MODE,
    I2S_WORD_SELECT,
    I2S_MODE,
    GPIO1_CONTROL,
    GPIO2_CONTROL,
    GPIO3_CONTROL,
    SEEK_MODE,
    for_each_conf,
)

WorkModeSelect = rda5820_ns.class_("WorkModeSelect", select.Select)
LnaPortSelSelect = rda5820_ns.class_("LnaPortSelSelect", select.Select)
DeEmphasisSelect = rda5820_ns.class_("DeEmphasisSelect", select.Select)
ClkModeSelect = rda5820_ns.class_("ClkModeSelect", select.Select)
I2sSwCntSelect = rda5820_ns.class_("I2sSwCntSelect", select.Select)
I2sModeSelect = rda5820_ns.class_("I2sModeSelect", select.Select)
Gpio1Select = rda5820_ns.class_("Gpio1Select", select.Select)
Gpio2Select = rda5820_ns.class_("Gpio2Select", select.Select)
Gpio3Select = rda5820_ns.class_("Gpio3Select", select.Select)
SeekModeSelect = rda5820_ns.class_("SeekModeSelect", select.Select)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_RDA5820_ID): cv.use_id(RDA5820Component),
        cv.Optional(CONF_WORK_MODE): select.select_schema(
            WorkModeSelect,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_LNA_PORT_SEL): select.select_schema(
            LnaPortSelSelect,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_DE_EMPHASIS): select.select_schema(
            DeEmphasisSelect,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_CLK_MODE): select.select_schema(
            ClkModeSelect,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_SW_CNT): select.select_schema(
            I2sSwCntSelect,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_MODE): select.select_schema(
            I2sModeSelect,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_GPIO1): select.select_schema(
            Gpio1Select,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_GPIO2): select.select_schema(
            Gpio2Select,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_GPIO3): select.select_schema(
            Gpio3Select,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
        cv.Optional(CONF_SEEK_MODE): select.select_schema(
            SeekModeSelect,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # icon=ICON_,
        ),
    }
)

VARIABLES = {
    None: [
        [CONF_WORK_MODE, WORK_MODE],
        [CONF_LNA_PORT_SEL, LNA_INPUT_PORT],
        [CONF_DE_EMPHASIS, DE_EMPHASIS],
        [CONF_CLK_MODE, CLK_MODE],
        [CONF_SW_CNT, I2S_WORD_SELECT],
        [CONF_MODE, I2S_MODE],
        [CONF_GPIO1, GPIO1_CONTROL],
        [CONF_GPIO2, GPIO2_CONTROL],
        [CONF_GPIO3, GPIO3_CONTROL],
        [CONF_SEEK_MODE, SEEK_MODE],
    ],
}


async def to_code(config):
    parent = await cg.get_variable(config[CONF_RDA5820_ID])

    async def new_select(c, args, setter):
        s = await select.new_select(c, options=list(args[1].keys()))
        await cg.register_parented(s, parent)
        cg.add(getattr(parent, setter + "_select")(s))

    await for_each_conf(config, VARIABLES, new_select)
