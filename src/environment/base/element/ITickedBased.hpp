#pragma once

#include "../../../rendering/Globals.hpp"

using namespace std;

namespace BaseInterface {

class ITickedBase {
    public:
        virtual void setTick(Tick tick) noexcept;
        virtual void update() noexcept;
        Tick getTick() const&;
    private:
        Tick tick;
};

void ITickedBase::setTick(Tick tick) noexcept {
    this->tick = tick;
};

void ITickedBase::update() noexcept {
    this->tick++;
};

Tick ITickedBase::getTick() const& {
    return tick;
};
}