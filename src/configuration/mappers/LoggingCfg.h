#pragma once

using namespace std;

class LoggingCfg {
    public:
     LoggingCfg();
     LoggingCfg(bool gl_debug, bool player_pos, bool tex_processing, bool map_processing, bool hide_warnings, bool hide_infos);
     ~LoggingCfg();

     bool gl_debug;
     bool player_pos;
     bool tex_processing;
     bool map_processing;
     bool hide_warnings;
     bool hide_infos;
};

LoggingCfg::LoggingCfg() {};

LoggingCfg::LoggingCfg(bool gl_debug, bool player_pos, bool tex_processing, bool map_processing, bool hide_warnings, bool hide_infos) {
    this->gl_debug = gl_debug;
    this->player_pos = player_pos;
    this->tex_processing = tex_processing;
    this->map_processing = map_processing;
    this->hide_warnings = hide_warnings;
    this->hide_infos = hide_infos;
};

LoggingCfg::~LoggingCfg(){};