#pragma once

#include <string>
#include <functional>

#include "../../rendering/buffering/PBO.hpp"
#include "../../rendering/colour/Colours.hpp"
#include "../../rendering/drawing/RasterText.hpp"
#include "../../rendering/drawing/DrawingUtils.hpp"
#include "../base/IBaseElement.hpp"
#include "../../logic/id/IDGenerator.hpp"
#include "../../rendering/Globals.hpp"

using namespace std;

namespace GUI {

enum BUTTON_STATE : int {
    CLICKED = 0,
    NOT_PRESSED = 1,
    HOVERED = 2
};

typedef function<void(int_id)> ButtonCallback;

class Button : public IBaseElement {
    public:
        Button();
        Button(string text, ButtonCallback callback);
        Button(int x, int y, int width, int height, string text, ButtonCallback callback);
        Button(int x, int y, int width, int height, string text, Colour::ColorRGB background_colour, Colour::ColorRGB hover_colour, Colour::ColorRGB click_colour, ButtonCallback callback);

        void render(Rendering::PBO& pbo);
        void render();

        void setBackgroundColour(Colour::ColorRGB colour);
        void setHoverColour(Colour::ColorRGB colour);
        void setClickColour(Colour::ColorRGB colour);

        Colour::ColorRGB getBackgroundColour();
        Colour::ColorRGB getHoverColour();
        Colour::ColorRGB getClickColour();

        void setX(int x);
        void setY(int y);
        void setWidth(int width);
        void setHeight(int height);
        void setText(string text);

        int getX();
        int getY();
        int getWidth();
        int getHeight();
        string getText();
        void handleMouse(int button, int state, int posx, int posy);
    private:
        bool inside(int posx, int ypos);

        string text;
        BUTTON_STATE state;
        Colour::ColorRGB hover_colour;
        Colour::ColorRGB click_colour;
        ButtonCallback callback;
};

Button::Button():
    Button(0,0,0,0, "Button", [](int_id id){})
{};

Button::Button(string text, ButtonCallback callback):
    Button(0,0,0,0, text, callback)
{};

Button::Button(int x, int y, int width, int height, string text, ButtonCallback callback):
    Button(x, y, width, height, text, Colour::RGB_Grey, Colour::RGB_White, Colour::RGB_Black, callback)
{};

Button::Button(int x, int y, int width, int height, string text, Colour::ColorRGB background_colour, Colour::ColorRGB hover_colour, Colour::ColorRGB click_colour, ButtonCallback callback):
    IBaseElement(x, y, width, height, background_colour)
{
    this->text = text;
    this->hover_colour = hover_colour;
    this->click_colour = click_colour;
    this->callback = callback;
};

void Button::setBackgroundColour(Colour::ColorRGB colour) {
    this->background_colour = colour;
};

void Button::setHoverColour(Colour::ColorRGB colour) {
    this->hover_colour = colour;
};

void Button::setClickColour(Colour::ColorRGB colour) {
    this->click_colour = colour;
};

void Button::setX(int x) {
    this->x = x;
};

void Button::setY(int y) {
    this->y = y;
};

void Button::setWidth(int width) {
    this->width = width;
};

void Button::setHeight(int height) {
    this->height = height;
};

void Button::setText(string text) {
    this->text = text;
};

Colour::ColorRGB Button::getBackgroundColour() {
    return this->background_colour;
};

Colour::ColorRGB Button::getHoverColour() {
    return this->hover_colour;
};

Colour::ColorRGB Button::getClickColour() {
    return this->click_colour;
};

int Button::getX() {
    return this->x;
};

int Button::getY() {
    return this->y;
};

int Button::getWidth() {
    return this->width;
};

int Button::getHeight() {
    return this->height;
};

string Button::getText() {
    return this->text;
};

void Button::render(Rendering::PBO& pbo) {
    Colour::ColorRGB render_colour = this->background_colour;
    switch (this->state) {
        case BUTTON_STATE::CLICKED:
            render_colour = this->click_colour;
            break;
        case BUTTON_STATE::HOVERED:
            render_colour = this->hover_colour;
            break;
        case BUTTON_STATE::NOT_PRESSED:
            render_colour = this->background_colour;
            break;
        default:
            render_colour = this->background_colour;
    }
    for (int x_pos = this->x; x_pos < this->x + this->width; x_pos++) {
        for (int y_pos = this->y; y_pos < this->y + this->height; y_pos++) {
            pbo.pushToBuffer(x_pos, y_pos, render_colour);
        }
    }
    displayText(IDIV_2(this->width - glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (unsigned char*) this->text.c_str())),
        IDIV_2(this->height - 18),
        Colour::RGB_Red, GLUT_BITMAP_HELVETICA_18, this->text);
};

void Button::render() {
    Colour::ColorRGB render_colour = this->background_colour;
    switch (this->state) {
        case BUTTON_STATE::CLICKED:
            render_colour = this->click_colour;
            break;
        case BUTTON_STATE::HOVERED:
            render_colour = this->hover_colour;
            break;
        case BUTTON_STATE::NOT_PRESSED:
            render_colour = this->background_colour;
            break;
    }
    GLint current_colour[4];
    glGetIntegerv(GL_CURRENT_COLOR, current_colour);
    render_colour.toColour4d();
    drawRectangle(this->x, this->y, this->width, this->height);
    glColor3i(
        current_colour[0],
        current_colour[1],
        current_colour[2]
    );
    displayText(IDIV_2(this->width - glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (unsigned char*) this->text.c_str())),
        IDIV_2(this->height - 18),
        Colour::RGB_Red, GLUT_BITMAP_HELVETICA_18, this->text);
}

bool Button::inside(int posx, int posy) {
    return (this->x <= posx && posx <= this->x + this->width
        &&  this->y <= posy && posy <= this->y + this->height);
}

void Button::handleMouse(int button, int state, int posx, int posy) {
    if (inside(posx, posy)) {
        if (state == GLUT_DOWN && (button == GLUT_LEFT_BUTTON || button == GLUT_RIGHT_BUTTON || button == GLUT_MIDDLE_BUTTON)) {
            this->state = BUTTON_STATE::CLICKED;
            this->callback(this->id);
        } else {
            this->state = BUTTON_STATE::HOVERED;
        }
    } else {
        this->state = BUTTON_STATE::NOT_PRESSED;
    }
}
}