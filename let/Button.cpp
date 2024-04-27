#include "Button.h"

Button::Button() :
	_hovered(false),
	_enabled(true),
	_pressed(false),
	_on_click(nullptr)
{
}

Button::Button(std::function<void()> onClick) :
	_hovered(false),
	_enabled(true),
	_pressed(false),
	_on_click(onClick)
{
}

void Button::input_mouse_move(int mouse_x, int mouse_y, bool& covered)
{
	_hovered = !covered && Element::check_hovered();
}

void Button::input_mouse_left_down(bool& cancel)
{
	if (cancel) return;	
	if (!_hovered) return;
	if (_on_click) this->_on_click();
	_pressed = true;
}

void Button::input_mouse_left_up(bool& cancel) {
	_pressed = false;
}

bool Button::is_enabled() const { return _enabled; }

bool Button::is_hovered() const { return _hovered;}
bool Button::is_pressed() const { return _pressed; }
