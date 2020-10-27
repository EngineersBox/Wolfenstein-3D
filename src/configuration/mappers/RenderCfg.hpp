#pragma once

using namespace std;

class RenderCfg {
    public:
     RenderCfg();
     RenderCfg(bool headless_mode, bool double_buffer, int refresh_rate, int ray_count);

     bool headless_mode;
     bool double_buffer;
     int refresh_rate;
     int ray_count;
};

RenderCfg::RenderCfg(){};

RenderCfg::RenderCfg(bool headless_mode, bool double_buffer, int refresh_rate, int ray_count) {
    this->headless_mode = headless_mode;
    this->double_buffer = double_buffer;
    this->refresh_rate = refresh_rate;
    this->ray_count = ray_count;
};
