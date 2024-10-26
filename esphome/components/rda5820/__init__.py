import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import i2c, sensor, binary_sensor, text_sensor
from esphome.const import (
    CONF_ID,
    CONF_SENSOR,
    CONF_FREQUENCY,
    CONF_VOLUME,
    CONF_OUTPUT,
    CONF_MODE,
    UNIT_DECIBEL,
    ICON_CHIP,
    DEVICE_CLASS_FREQUENCY,
    DEVICE_CLASS_SIGNAL_STRENGTH,
    STATE_CLASS_MEASUREMENT,
    ENTITY_CATEGORY_DIAGNOSTIC,
)

CODEOWNERS = ["@gabest11"]
DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["sensor", "binary_sensor", "text_sensor", "number", "switch", "select", "text"]
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

CONF_WORK_MODE = "work_mode"
CONF_LNA_PORT_SEL = "lna_port_sel"

# CONF_FREQUENCY = "frequency"
CONF_DE_EMPHASIS = "de_emphasis"
# CONF_VOLUME = "volume"
CONF_MONO = "mono"
CONF_MUTE = "mute"
CONF_SOFTMUTE = "softmute"
CONF_BASS_BOOST = "bass_boost"
CONF_AUDIO_HI_Z = "audio_hi_z"
CONF_POWER_ENABLE = "power_enable"
CONF_AFC_ENABLE = "afc_enable"
CONF_RBDS_ENABLE = "rbds_enable"
CONF_SOFTBLEND_ENABLE = "softblend_enable"
CONF_TH_SOFTBLEND = "th_softblend"
CONF_CLK_MODE = "clk_mode"
CONF_RCLK_DIRECT_INPUT_MODE = "rclk_direct_input_mode"
CONF_RCLK_NON_CALIBRATE_MODE = "rclk_non_calibrate_mode"
CONF_AUDIO_DEV = "audio_dev"
CONF_PILOT_DEV = "pilot_dev"
CONF_DEVIATION = "deviation"
CONF_PGA_GAIN = "pga_gain"
CONF_ADC_GAIN = "adc_gain"
CONF_PA_GAIN = "pa_gain"
CONF_PA_IBIT = "pa_ibit"
CONF_PA_VCOM = "pa_vcom"
CONF_SEEK_MODE = "seek_mode"
CONF_SEEK_TH = "seek_th"
CONF_SEEK_TH_OLD = "seek_th_old"
CONF_FREQ_MODE = "freq_mode"
# I2S
CONF_I2S = "i2s"
# CONF_ENABLE = "enable"
CONF_L_DELAY = "l_delay"
CONF_R_DELAY = "r_delay"
CONF_SCLK_O_EDGE = "sclk_o_edge"
CONF_SW_O_EDGE = "sw_o_edge"
CONF_WS_I_EDGE = "ws_i_edge"
CONF_DATA_SIGNED = "data_signed"
CONF_SCLK_I_EDGE = "sclk_i_edge"
CONF_SW_LR = "sw_lr"
CONF_SW_CNT = "sw_cnt"
# CONF_MODE = "mode"
# RDS
CONF_RDS = "rds"
CONF_ENABLE = "enable"
# CONF_DEVIATION = "deviation"
CONF_STATION = "station"
CONF_TEXT = "text"
# output
# CONF_OUTPUT = "output"
CONF_GPIO1 = "gpio1"
CONF_GPIO2 = "gpio2"
CONF_GPIO3 = "gpio3"

# sensor
CONF_CHIP_ID = "chip_id"
# CONF_FREQUENCY = "frequency"
CONF_RSSI = "rssi"
CONF_STEREO = "stereo"
CONF_SEEK_FAIL = "seek_fail"
CONF_SEEK_TUNE_COMPLETE = "seek_tune_complete"
CONF_FM_READY = "fm_ready"
CONF_FM_IS_STATION = "fm_is_station"
CONF_BLK_E_FOUND = "blk_e_found"
CONF_RDS_DECODER_SYNC = "rds_decoder_sync"

# action
CONF_SEEK_UP = "seek_up"
CONF_WRAP_AROUND = "wrap_around"

SetFrequencyAction = rda5820_ns.class_(
    "SetFrequencyAction", automation.Action, cg.Parented.template(RDA5820Component)
)

SeekAction = rda5820_ns.class_(
    "SeekAction", automation.Action, cg.Parented.template(RDA5820Component)
)

WorkMode = rda5820_ns.enum("WorkMode", True)
WORK_MODE = {
    "Receive": WorkMode.WORK_MODE_FM_RECEIVE,
    "Transmit": WorkMode.WORK_MODE_FM_TRANSMIT,
    "Amplify": WorkMode.WORK_MODE_AUDIO_AMPLIFY,
    "Codec": WorkMode.WORK_MODE_CODEC,
    "ADC": WorkMode.WORK_MODE_ADC,
}

LnaInputPort = rda5820_ns.enum("LnaInputPort", True)
LNA_INPUT_PORT = {
    "None": LnaInputPort.LNA_PORT_SEL_NO_INPUT,
    "LNAN": LnaInputPort.LNA_PORT_SEL_LNAN,
    "LNAP": LnaInputPort.LNA_PORT_SEL_LNAP,
    "Dual": LnaInputPort.LNA_PORT_SEL_DUAL_PORT_INPUT,
}

DeEmphasis = rda5820_ns.enum("DeEmphasis", True)
DE_EMPHASIS = {
    "75us": DeEmphasis.DE_75US,
    "50us": DeEmphasis.DE_50US,
}

ClkMode = rda5820_ns.enum("ClkMode", True)
CLK_MODE = {
    "32.768kHz": ClkMode.CLK_MODE_32K768,
    "12MHz": ClkMode.CLK_MODE_12M,
    "24MHz": ClkMode.CLK_MODE_24M,
    "13MHz": ClkMode.CLK_MODE_13M,
    "26MHz": ClkMode.CLK_MODE_26M,
    "19.2MHz": ClkMode.CLK_MODE_19M2,
    "38.4MHz": ClkMode.CLK_MODE_38M4,
}

I2sWordSelect = rda5820_ns.enum("I2sWordSelect", True)
I2S_WORD_SELECT = {
    "8000": I2sWordSelect.I2S_SW_CNT_8KBPS,
    "11025": I2sWordSelect.I2S_SW_CNT_11K025BPS,
    "12000": I2sWordSelect.I2S_SW_CNT_12KBPS,
    "16000": I2sWordSelect.I2S_SW_CNT_16KBPS,
    "22050": I2sWordSelect.I2S_SW_CNT_22K05BPS,
    "24000": I2sWordSelect.I2S_SW_CNT_24KBPS,
    "32000": I2sWordSelect.I2S_SW_CNT_32KBPS,
    "44100": I2sWordSelect.I2S_SW_CNT_44K1BPS,
    "48000": I2sWordSelect.I2S_SW_CNT_48KBPS,
}

I2sMode = rda5820_ns.enum("I2sMode", True)
I2S_MODE = {
    "Master": I2sMode.I2S_MODE_MASTER,
    "Slave": I2sMode.I2S_MODE_SLAVE,
}

Gpio1Control = rda5820_ns.enum("Gpio1Control", True)
GPIO1_CONTROL = {
    "High-Z": Gpio1Control.GPIO_HIGH_Z,
    "Reserved": Gpio1Control.GPIO_RESERVED,
    "Low": Gpio1Control.GPIO_LOW,
    "High": Gpio1Control.GPIO_HIGH,
}

Gpio2Control = rda5820_ns.enum("Gpio2Control", True)
GPIO2_CONTROL = {
    "High-Z": Gpio2Control.GPIO_HIGH_Z,
    "Interrupt": Gpio2Control.GPIO_INTERRUPT,
    "Low": Gpio2Control.GPIO_LOW,
    "High": Gpio2Control.GPIO_HIGH,
}

Gpio3Control = rda5820_ns.enum("Gpio3Control", True)
GPIO3_CONTROL = {
    "High-Z": Gpio3Control.GPIO_HIGH_Z,
    "Stereo": Gpio3Control.GPIO_STEREO_INDICATOR,
    "Low": Gpio3Control.GPIO_LOW,
    "High": Gpio3Control.GPIO_HIGH,
}

SeekMode = rda5820_ns.enum("SeekMode", True)
SEEK_MODE = {
    "Default": SeekMode.SEEK_MODE_DEFAULT,
    "And": SeekMode.SEEK_MODE_AND,
    "Or": SeekMode.SEEK_MODE_OR,
}

I2S_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_ENABLE, default=False): cv.boolean,
        cv.Optional(CONF_L_DELAY, default=False): cv.boolean,
        cv.Optional(CONF_R_DELAY, default=False): cv.boolean,
        cv.Optional(CONF_SCLK_O_EDGE, default=False): cv.boolean,
        cv.Optional(CONF_SW_O_EDGE, default=False): cv.boolean,
        cv.Optional(CONF_SW_CNT, default="8000"): cv.enum(I2S_WORD_SELECT),
        cv.Optional(CONF_WS_I_EDGE, default=False): cv.boolean,
        cv.Optional(CONF_DATA_SIGNED, default=False): cv.boolean,
        cv.Optional(CONF_SCLK_I_EDGE, default=False): cv.boolean,
        cv.Optional(CONF_SW_LR, default=True): cv.boolean,
        cv.Optional(CONF_MODE, default="Master"): cv.enum(I2S_MODE),
    }
)

RDS_SCHEMA = cv.Schema(
    {
        cv.Optional(CONF_ENABLE, default=False): cv.boolean,
        cv.Optional(CONF_DEVIATION, default=16): cv.int_range(0, 255),
        cv.Optional(CONF_STATION): cv.string,
        cv.Optional(CONF_TEXT): cv.string,
    }
)

SENSOR_SCHEMA = (
    {
        cv.Optional(CONF_CHIP_ID): text_sensor.text_sensor_schema(
            # unit_of_measurement=UNIT_,
            # accuracy_decimals=3,
            # device_class=DEVICE_CLASS_,
            # state_class=STATE_CLASS_MEASUREMENT,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            icon=ICON_CHIP,
        ),
        cv.Optional(CONF_FREQUENCY): sensor.sensor_schema(
            unit_of_measurement=UNIT_MEGA_HERTZ,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_FREQUENCY,
            state_class=STATE_CLASS_MEASUREMENT,
            # entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            # icon=ICON_CHIP,
        ),
        cv.Optional(CONF_RSSI): sensor.sensor_schema(
            unit_of_measurement=UNIT_DECIBEL,  # might be UNIT_DECIBEL_MILLIWATT
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_SIGNAL_STRENGTH,
            state_class=STATE_CLASS_MEASUREMENT,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            # icon=ICON_CHIP,
        ),
        # TODO
        cv.Optional(CONF_STEREO): binary_sensor.binary_sensor_schema(
            # unit_of_measurement=UNIT_,
            # accuracy_decimals=3,
            # device_class=DEVICE_CLASS_,
            # state_class=STATE_CLASS_MEASUREMENT,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            icon=ICON_CHIP,
        ),
        cv.Optional(CONF_SEEK_FAIL): binary_sensor.binary_sensor_schema(
            # unit_of_measurement=UNIT_,
            # accuracy_decimals=3,
            # device_class=DEVICE_CLASS_,
            # state_class=STATE_CLASS_MEASUREMENT,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            icon=ICON_CHIP,
        ),
        cv.Optional(CONF_SEEK_TUNE_COMPLETE): binary_sensor.binary_sensor_schema(
            # unit_of_measurement=UNIT_,
            # accuracy_decimals=3,
            # device_class=DEVICE_CLASS_,
            # state_class=STATE_CLASS_MEASUREMENT,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            icon=ICON_CHIP,
        ),
        cv.Optional(CONF_FM_READY): binary_sensor.binary_sensor_schema(
            # unit_of_measurement=UNIT_,
            # accuracy_decimals=3,
            # device_class=DEVICE_CLASS_,
            # state_class=STATE_CLASS_MEASUREMENT,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            icon=ICON_CHIP,
        ),
        cv.Optional(CONF_FM_IS_STATION): binary_sensor.binary_sensor_schema(
            # unit_of_measurement=UNIT_,
            # accuracy_decimals=3,
            # device_class=DEVICE_CLASS_,
            # state_class=STATE_CLASS_MEASUREMENT,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            icon=ICON_CHIP,
        ),
        cv.Optional(CONF_BLK_E_FOUND): binary_sensor.binary_sensor_schema(
            # unit_of_measurement=UNIT_,
            # accuracy_decimals=3,
            # device_class=DEVICE_CLASS_,
            # state_class=STATE_CLASS_MEASUREMENT,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            icon=ICON_CHIP,
        ),
        cv.Optional(CONF_RDS_DECODER_SYNC): binary_sensor.binary_sensor_schema(
            # unit_of_measurement=UNIT_,
            # accuracy_decimals=3,
            # device_class=DEVICE_CLASS_,
            # state_class=STATE_CLASS_MEASUREMENT,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            icon=ICON_CHIP,
        ),
    }
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(RDA5820Component),
            cv.Required(CONF_WORK_MODE): cv.enum(WORK_MODE),
            cv.Optional(CONF_LNA_PORT_SEL, default="LNAP"): cv.enum(LNA_INPUT_PORT),
            cv.Optional(CONF_FREQUENCY, default=87.5): cv.float_range(50, 108),
            cv.Optional(CONF_DE_EMPHASIS, default="75us"): cv.enum(DE_EMPHASIS),
            cv.Optional(CONF_VOLUME, default=0.5): cv.percentage,
            cv.Optional(CONF_MONO, default=False): cv.boolean,
            cv.Optional(CONF_MUTE, default=False): cv.boolean,
            cv.Optional(CONF_SOFTMUTE, default=False): cv.boolean,
            cv.Optional(CONF_BASS_BOOST, default=False): cv.boolean,
            cv.Optional(CONF_AUDIO_HI_Z, default=True): cv.boolean,
            cv.Optional(CONF_POWER_ENABLE, default=True): cv.boolean,
            cv.Optional(CONF_AFC_ENABLE, default=True): cv.boolean,
            cv.Optional(CONF_RBDS_ENABLE, default=False): cv.boolean,
            cv.Optional(CONF_SOFTBLEND_ENABLE, default=True): cv.boolean,
            cv.Optional(CONF_TH_SOFTBLEND, default=19): cv.int_range(0, 31),
            cv.Optional(CONF_CLK_MODE, default="32.768kHz"): cv.enum(CLK_MODE),
            cv.Optional(CONF_RCLK_DIRECT_INPUT_MODE, default=False): cv.boolean,
            cv.Optional(CONF_RCLK_NON_CALIBRATE_MODE, default=False): cv.boolean,
            cv.Optional(CONF_AUDIO_DEV, default=240): cv.int_range(0, 255),
            cv.Optional(CONF_PILOT_DEV, default=14): cv.int_range(0, 255),
            cv.Optional(CONF_PGA_GAIN, default=1): cv.int_range(0, 7),
            cv.Optional(CONF_ADC_GAIN, default=0): cv.int_range(0, 7),
            cv.Optional(CONF_PA_GAIN, default=0): cv.int_range(0, 63),
            cv.Optional(CONF_PA_IBIT, default=7): cv.int_range(0, 7),
            cv.Optional(CONF_PA_VCOM, default=0): cv.int_range(0, 7),
            cv.Optional(CONF_SEEK_MODE, default="Default"): cv.enum(SEEK_MODE),
            cv.Optional(CONF_SEEK_TH, default=8): cv.int_range(0, 15),
            cv.Optional(CONF_SEEK_TH_OLD, default=0): cv.int_range(0, 63),
            cv.Optional(CONF_FREQ_MODE, default=False): cv.boolean,
            cv.Optional(CONF_GPIO1, default="High-Z"): cv.enum(GPIO1_CONTROL),
            cv.Optional(CONF_GPIO2, default="High-Z"): cv.enum(GPIO2_CONTROL),
            cv.Optional(CONF_GPIO3, default="High-Z"): cv.enum(GPIO3_CONTROL),
            cv.Optional(CONF_I2S): I2S_SCHEMA,
            cv.Optional(CONF_RDS): RDS_SCHEMA,
            cv.Optional(CONF_SENSOR): SENSOR_SCHEMA,
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x2C))
)

VARIABLES = {
    None: [
        [CONF_WORK_MODE],
        [CONF_LNA_PORT_SEL],
        [CONF_FREQUENCY],
        [CONF_DE_EMPHASIS],
        [CONF_VOLUME],
        [CONF_MONO],
        [CONF_MUTE],
        [CONF_SOFTMUTE],
        [CONF_BASS_BOOST],
        [CONF_AUDIO_HI_Z],
        [CONF_POWER_ENABLE],
        [CONF_AFC_ENABLE],
        [CONF_RBDS_ENABLE],
        [CONF_SOFTBLEND_ENABLE],
        [CONF_TH_SOFTBLEND],
        [CONF_CLK_MODE],
        [CONF_RCLK_DIRECT_INPUT_MODE],
        [CONF_RCLK_NON_CALIBRATE_MODE],
        [CONF_AUDIO_DEV],
        [CONF_PILOT_DEV],
        [CONF_PGA_GAIN],
        [CONF_ADC_GAIN],
        [CONF_PA_GAIN],
        [CONF_PA_IBIT],
        [CONF_PA_VCOM],
        [CONF_SEEK_MODE],
        [CONF_SEEK_TH],
        [CONF_SEEK_TH_OLD],
        [CONF_FREQ_MODE],
        [CONF_GPIO1],
        [CONF_GPIO2],
        [CONF_GPIO3],
    ],
    CONF_I2S: [
        [CONF_ENABLE],
        [CONF_SW_CNT],
        [CONF_MODE],
        [CONF_L_DELAY],
        [CONF_R_DELAY],
        [CONF_SCLK_O_EDGE],
        [CONF_SW_O_EDGE],
        [CONF_WS_I_EDGE],
        [CONF_DATA_SIGNED],
        [CONF_SCLK_I_EDGE],
        [CONF_SW_LR],
    ],
    CONF_RDS: [
        [CONF_ENABLE],
        [CONF_DEVIATION],
        [CONF_STATION],
        [CONF_TEXT],
    ],
}

SENSORS = {
    CONF_SENSOR: [
        [CONF_CHIP_ID, "text_sensor"],
        [CONF_FREQUENCY, "sensor"],
        [CONF_RSSI, "sensor"],
        [CONF_STEREO, "binary_sensor"],
        [CONF_SEEK_FAIL, "binary_sensor"],
        [CONF_SEEK_TUNE_COMPLETE, "binary_sensor"],
        [CONF_FM_READY, "binary_sensor"],
        [CONF_FM_IS_STATION, "binary_sensor"],
        [CONF_BLK_E_FOUND, "binary_sensor"],
        [CONF_RDS_DECODER_SYNC, "binary_sensor"],
    ],
}


async def for_each_conf(config, vars, callback):
    for section in vars:
        if section is not None and section not in config:
            continue
        c = config[section] if section is not None else config
        for args in vars[section]:
            setter = "set_"
            if section is not None and section != CONF_SENSOR:
                setter += section + "_"
            setter += args[0]
            if args[0] in c:
                # print(setter + "(" + repr(c[args[0]]) + ")")
                await callback(c[args[0]], args, setter)


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
        elif args[1] == "binary_sensor":
            s = await binary_sensor.new_binary_sensor(c)
        elif args[1] == "text_sensor":
            s = await text_sensor.new_text_sensor(c)
        cg.add(getattr(var, setter + "_" + args[1])(s))

    await for_each_conf(config, SENSORS, new_sensor)


FREQUENCY_SCHEMA = automation.maybe_simple_id(
    {
        cv.GenerateID(): cv.use_id(RDA5820Component),
        cv.Required(CONF_FREQUENCY): cv.templatable(cv.float_range(min=50, max=108)),
    }
)

SEEK_SCHEMA = automation.maybe_simple_id(
    {
        cv.GenerateID(): cv.use_id(RDA5820Component),
        cv.Optional(CONF_SEEK_UP, default=True): cv.templatable(cv.boolean),
        cv.Optional(CONF_WRAP_AROUND, default=False): cv.templatable(cv.boolean),
    }
)


@automation.register_action("rda5820.set_frequency", SetFrequencyAction, FREQUENCY_SCHEMA)
async def set_frequency_action_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])
    frequency = config.get(CONF_FREQUENCY)
    template_ = await cg.templatable(frequency, args, cg.float_)
    cg.add(var.set_frequency(template_))
    return var


@automation.register_action("rda5820.seek", SeekAction, SEEK_SCHEMA)
async def seek_action_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])
    seek_up = config.get(CONF_SEEK_UP)
    seek_up_template_ = await cg.templatable(seek_up, args, cg.bool_)
    wrap_around = config.get(CONF_WRAP_AROUND)
    wrap_around_template_ = await cg.templatable(wrap_around, args, cg.bool_)
    cg.add(var.set_seek_up(seek_up_template_))
    cg.add(var.set_wrap_around(wrap_around_template_))
    return var
