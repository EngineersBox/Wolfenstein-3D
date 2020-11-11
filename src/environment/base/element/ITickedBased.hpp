#pragma once

#include "../../../rendering/Globals.hpp"

using namespace std;

namespace BaseInterface {

struct ITickedBase {
    virtual void setTick(Tick tick) noexcept;
    virtual void update();
    Tick getTick() const&;
    Tick tick;
};

void ITickedBase::setTick(Tick tick) noexcept {
    this->tick = tick;
};

void ITickedBase::update() {
    this->tick++;
};

Tick ITickedBase::getTick() const& {
    return tick;
};
}