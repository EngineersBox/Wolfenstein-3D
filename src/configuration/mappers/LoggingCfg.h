#pragma once

using namespace std;

class LoggingCfg {
    public:
     LoggingCfg();
     LoggingCfg(bool gl_debug, bool player_pos, bool tex_skip_invalid, bool map_skip_invalid, bool hide_warnings, bool hide_infos, bool log_verbose);
     ~LoggingCfg();

     bool gl_debug;
     bool player_pos;
     bool tex_skip_invalid;
     bool map_skip_invalid;
     bool hide_warnings;
     bool hide_infos;
     bool log_verbose;
};

LoggingCfg::LoggingCfg() {};

LoggingCfg::LoggingCfg(bool gl_debug, bool player_pos, bool tex_skip_invalid, bool map_skip_invalid, bool hide_warnings, bool hide_infos, bool log_verbose) {
    this->gl_debug = gl_debug;
    this->player_pos = player_pos;
    this->tex_skip_invalid = tex_skip_invalid;
    this->map_skip_invalid = map_skip_invalid;
    this->hide_warnings = hide_warnings;
    this->hide_infos = hide_infos;
    this->log_verbose = log_verbose;
};

LoggingCfg::~LoggingCfg(){};