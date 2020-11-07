#pragma once

using namespace std;

namespace ConfigSection {

struct RenderCfg {
    bool headless_mode;
    bool double_buffer;
    bool render_walls;
    bool render_floor_ceiling;
    bool render_sprites;
    int refresh_rate;
    int ray_count;
    int render_distance;
    int texture_width;
    int texture_height;
    bool show_stats_bar;
};
}