#pragma once

using namespace std;

class LoggingCfg {
    public:
     LoggingCfg();
     LoggingCfg(bool gl_debug, bool player_pos, bool tex_processing, bool map_processing);
     ~LoggingCfg();

     bool gl_debug;
     bool player_pos;
     bool tex_processing;
     bool map_processing;
};

LoggingCfg::LoggingCfg() {};

LoggingCfg::LoggingCfg(bool gl_debug, bool player_pos, bool tex_processing, bool map_processing) {
    this->gl_debug = gl_debug;
    this->player_pos = player_pos;
    this->tex_processing = tex_processing;
    this->map_processing = map_processing;
};

LoggingCfg::~LoggingCfg(){};