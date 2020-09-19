#pragma once

using namespace std;

class RenderCfg {
    public:
     RenderCfg();
     RenderCfg(bool headless_mode, bool double_buffer, int refresh_rate);
     ~RenderCfg();

     bool headless_mode;
     bool double_buffer;
     int refresh_rate;
};

RenderCfg::RenderCfg(){};

RenderCfg::RenderCfg(bool headless_mode, bool double_buffer, int refresh_rate) {
    this->headless_mode = headless_mode;
    this->double_buffer = double_buffer;
    this->refresh_rate = refresh_rate;
};

RenderCfg::~RenderCfg(){};