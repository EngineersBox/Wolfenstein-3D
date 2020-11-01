#pragma once

using namespace std;

class RenderCfg {
    public:
     RenderCfg();
     RenderCfg(bool headless_mode, bool double_buffer, bool render_walls, bool render_floor_ceiling, bool render_sprites, int refresh_rate, int ray_count);

     bool headless_mode;
     bool double_buffer;
     bool render_walls;
     bool render_floor_ceiling;
     bool render_sprites;
     int refresh_rate;
     int ray_count;
};

RenderCfg::RenderCfg(){};

RenderCfg::RenderCfg(bool headless_mode, bool double_buffer, bool render_walls, bool render_floor_ceiling, bool render_sprites, int refresh_rate, int ray_count) {
    this->headless_mode = headless_mode;
    this->double_buffer = double_buffer;
    this->render_walls = render_walls;
    this->render_floor_ceiling = render_floor_ceiling;
    this->render_sprites = render_sprites;
    this->refresh_rate = refresh_rate;
    this->ray_count = ray_count;
};
