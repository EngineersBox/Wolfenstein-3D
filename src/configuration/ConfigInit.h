#pragma once

#include "INIReader.h"
#include "../exceptions/config/INIReadError.h"
#include "MinimapCfg.h"
#include "LoggingCfg.h"
#include "PlayerCfg.h"
using namespace std;

const string CFG_DIR = "resources/configs/";
#define DEFAULT_CONFIG CFG_DIR + "config.ini"

#define PLAYER_SECTION "player"
#define MINIMAP_SECTION "minimap"
#define LOGGING_SECTION "logging"

class ConfigInit {
    private:
        INIReader reader;
        string config_file;

    public:
        ConfigInit(const string& cfg_file = DEFAULT_CONFIG);
        ~ConfigInit();

        void processCfg();

        PlayerCfg initPlayerConfig();
        MinimapCfg initMinimapConfig();
        LoggingCfg initLoggingConfig();
        void initAll(PlayerCfg& p_cfg, MinimapCfg& m_cfg, LoggingCfg& l_cfg);
};

ConfigInit::ConfigInit(const string& cfg_file) {
    this->config_file = cfg_file;
    processCfg();
};

ConfigInit::~ConfigInit() {};

void ConfigInit::processCfg() {
    this->reader = INIReader(config_file);
    if (this->reader.ParseError() < 0) {
        throw INIReadError("[" + config_file + "] Can't load INI file");
    }
}

PlayerCfg ConfigInit::initPlayerConfig() {
    return PlayerCfg(
        reader.GetFloat(PLAYER_SECTION, "fov", 70.0f),
        reader.GetFloat(PLAYER_SECTION, "dof", 8.0f),
        reader.GetFloat(PLAYER_SECTION, "sensitivity", 2.0f)
    );
};

MinimapCfg ConfigInit::initMinimapConfig() {
    return MinimapCfg(
        reader.GetBoolean(MINIMAP_SECTION, "enable", false),
        reader.GetBoolean(MINIMAP_SECTION, "render_rays", true),
        parseMinimapPos(reader.Get(MINIMAP_SECTION, "pos", "TOP_LEFT"))
    );
};

LoggingCfg ConfigInit::initLoggingConfig() {
    return LoggingCfg(
        reader.GetBoolean(LOGGING_SECTION, "gl_debug", false),
        reader.GetBoolean(LOGGING_SECTION, "player_pos", false),
        reader.GetBoolean(LOGGING_SECTION, "tex_processing", false),
        reader.GetBoolean(LOGGING_SECTION, "map_processing", false)
    );
};

void ConfigInit::initAll(PlayerCfg& p_cfg, MinimapCfg& m_cfg, LoggingCfg& l_cfg) {
    p_cfg = initPlayerConfig();
    m_cfg = initMinimapConfig();
    l_cfg = initLoggingConfig();
}