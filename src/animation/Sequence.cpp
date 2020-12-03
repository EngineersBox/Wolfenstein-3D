#pragma once

#include <vector>
#include <string>

#include "../environment/base/element/ITickedBased.cpp"

using namespace std;

namespace Animation {

class Sequence : public BaseInterface::ITickedBase {
    public:
        Sequence();
        Sequence(vector<string> frames, vector<Tick> durations);

        string nextFrame();
        void reset();
    private:
        int idx = 0;

        vector<string> frames;
        vector<Tick> durations;
};

Sequence::Sequence(){};

Sequence::Sequence(vector<string> frames, vector<Tick> durations) {
    this->frames = frames;
    this->durations = durations;
};

string Sequence::nextFrame() {
    if (this->getTick() >= this->durations[this->idx]) {
        this->idx = (this->idx + 1) % (this->frames.size() - 1);
        this->setTick(0);
        return this->frames[this->idx];
    }
    this->update();
    return this->frames[this->idx];
};

void Sequence::reset() {
    this->idx = 0;
    this->setTick(0);
};

};