#pragma once

using namespace std;

struct LoggingCfg {
    bool gl_debug;
    bool tex_skip_invalid;
    bool map_skip_invalid;
    bool hide_warnings;
    bool hide_infos;
    bool log_verbose;
    bool show_fps;
    bool show_player_pos;
    bool show_time_tick;
};