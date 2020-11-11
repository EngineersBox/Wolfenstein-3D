#pragma once

#include <unordered_map>

#include "base/IBaseElement.hpp"

using namespace std;

namespace GUI {

class Canvas {
    public:
        Canvas(){};

        void addElement(IBaseElement &element);
        void removeElement(int_id id);
    private:
        unordered_map<int_id, IBaseElement> gui_elements;
};

void Canvas::addElement(IBaseElement &element) {
    this->gui_elements.emplace(element.id, element);
};

void Canvas::removeElement(int_id id) {
    this->gui_elements.erase(id);
};
}