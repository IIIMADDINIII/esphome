import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation, yaml_util
from esphome.core import CORE

from esphome.const import (
    CONF_ID,
)

CODEOWNERS = ["@gabest11"]
CONF_SHOW_IN_LOGCONFIG = "show_in_logconfig"
CONF_SHOW_SECRETS = "show_secrets"

store_yaml_ns = cg.esphome_ns.namespace("store_yaml")
StoreYamlComponent = store_yaml_ns.class_("StoreYamlComponent", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(StoreYamlComponent),
        cv.Optional(CONF_SHOW_IN_LOGCONFIG, default=True): cv.boolean,
        cv.Optional(CONF_SHOW_SECRETS, default=False): cv.boolean,
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    store_yaml = await cg.register_component(var, config)
    cg.add(store_yaml.set_show_in_logconfig(config[CONF_SHOW_IN_LOGCONFIG]))
    yaml = yaml_util.load_yaml(CORE.config_path)
    dump = yaml_util.dump(yaml, show_secrets=config[CONF_SHOW_SECRETS])
    cg.add(store_yaml.set_yaml(dump))


LogAction = store_yaml_ns.class_("LogAction", automation.Action)

LOG_ACTION_SCHEMA = automation.maybe_simple_id(
    {cv.GenerateID(): cv.use_id(StoreYamlComponent)}
)


@automation.register_action(
    "store_yaml.log", LogAction, LOG_ACTION_SCHEMA
)
async def log_action_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])
    return var
