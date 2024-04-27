#include "Element.h"

void Element::_set_state(UiState* state)
{
	this->_state = state;
}

void Element::_layout_update_bounds(int x, int y, int width, int height)
{
	if (width < 0) throw std::invalid_argument("width < 0");
	if (height < 0) throw std::invalid_argument("height < 0");
	this->_x = x;
	this->_y = y;
	this->_width = width;
	this->_height = height;
	this->on_bounds_updated();
}

void Element::_parent_set(Element* parent)
{
	this->_parent = parent;
	this->_set_state(parent->get_state());
	this->on_parent_set();
}

Element::Element() :
	_x(std::numeric_limits<int>::min()),
	_y(std::numeric_limits<int>::min()),
	_width(0),
	_height(0),
	_parent(nullptr),
	_state(nullptr)
{
}

bool Element::check_hovered()
{
	return _state->get_mouse_x() >= _x && _state->get_mouse_x() < _x + _width
		&& _state->get_mouse_y() >= _y && _state->get_mouse_y() < _y + _height;
}

int Element::get_x() const { return this->_x; }
int Element::get_y() const { return this->_y; }
int Element::get_width() const { return this->_width; }
int Element::get_height() const { return this->_height; }
int Element::get_right() const { return this->_x + this->_width; }
int Element::get_bottom() const { return this->_y + this->_height; }
int Element::current_x() const { return this->_x + get_state()->x_off(); }
int Element::current_y() const { return this->_y + get_state()->y_off(); }
int Element::current_right() const { return current_x() + this->_width; }
int Element::current_bottom() const { return current_y() + this->_height; }
Element* Element::get_parent() const { return this->_parent; }
UiState* Element::get_state() const { return _state; }

float Element::to_float(int i) { return static_cast<float>(i); }
int Element::math_round(float x) { return static_cast<int>(round(x)); }
int Element::math_floor(float x) { return static_cast<int>(floor(x)); }
int Element::math_ceil(float x) { return static_cast<int>(ceil(x)); }

SDL_Color Element::color(unsigned int rgba)
{
	return
	{
		static_cast<unsigned char>((rgba >> 0x18) & 0xff),
		static_cast<unsigned char>((rgba >> 0x10) & 0xff),
		static_cast<unsigned char>((rgba >> 0x08) & 0xff),
		static_cast<unsigned char>(rgba & 0xff)
	};
}

void Element::render_draw_outline(SDL_Renderer* renderer, int x, int y, int width, int height, Thickness thickness)
{
	SDL_Rect top{
		x,
		y,
		width,
		std::min(thickness.top, height)
	};
	SDL_RenderFillRect(renderer, &top);

	SDL_Rect right{
		x + width - thickness.right,
		y + thickness.top,
		thickness.right,
		height - thickness.ver()
	};
	SDL_RenderFillRect(renderer, &right);

	SDL_Rect bottom{
		x,
		y + height - thickness.bottom,
		width,
		thickness.bottom
	};
	SDL_RenderFillRect(renderer, &bottom);

	SDL_Rect left{
		x,
		y + thickness.top,
		thickness.left,
		height - thickness.ver()
	};
	SDL_RenderFillRect(renderer, &left);
}

void Element::render_set_color(SDL_Renderer* renderer, SDL_Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

int Element::align(int pos, int containerSize, int contentSize, Alignment alignment)
{
	switch (alignment)
	{
	case Start:
		return pos;
	case End:
		return pos + containerSize - contentSize;
	case Center:
		return pos + math_round(to_float(containerSize - contentSize) / 2.0f);
	default:
		throw std::runtime_error("unknown alignment: " + alignment);
	}
}
