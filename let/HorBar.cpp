#include "HorBar.h"

static int layoutMinBarSize = 24;
static int layoutMaxBarMargin = 24;

HorBar::HorBar(float value, float barScale) :
	_value(value),
	_bar_scale(barScale),
	_grabbed(false),
	_bar_hovered(false),
	_grab_x(std::numeric_limits<int>::min()),
	_grab_value(0)
{
}

void HorBar::on_bounds_updated()
{
}

void HorBar::render(SDL_Renderer* renderer)
{
	auto foreground = _bar_hovered ? color(0x787878ff) : color(0x606060ff);
	// auto& background = is_pressed() ? _pressed : (is_hovered() ? _hovered : _normal);
	auto background = color(0x181818ff);
	render_set_color(renderer, background);

	SDL_Rect rect{
		get_x(),
		get_y(),
		get_width(),
		get_height()
	};
	SDL_RenderFillRect(renderer, &rect);

	render_set_color(renderer, foreground);
	SDL_Rect rect2{
		bar_x(),
		get_y(),
		bar_width(),
		get_height()
	};
	SDL_RenderFillRect(renderer, &rect2);
}

void HorBar::input_mouse_move(int mouseX, int mouseY, bool& covered)
{
	if (_grabbed)
	{
		int dX = mouseX - _grab_x;
		_value = _grab_value + float(dX) / slide_width();
		if (_value < 0) _value = 0;
		if (_value >= 1) _value = 1;
	}

	int bx = bar_x(), by = get_y(), bw = bar_width(), bh = get_height();
	_bar_hovered = !covered && 
		  (mouseX >= bx && mouseX < bx + bw 
		&& mouseY >= by && mouseY < by + bh);
}

void HorBar::input_mouse_left_down(bool& canceled)
{
	if (canceled) return;
	if (!_bar_hovered) return;
	_grabbed = true;
	_grab_x = get_state()->get_mouse_x();
	_grab_value = _value;
}

void HorBar::input_mouse_left_up(bool& canceled)
{
	_grabbed = false;
}

void HorBar::update_bar_scale(float barScale)
{
	_bar_scale = barScale;
}

int HorBar::bar_width() const
{
	auto barWidth = math_ceil(get_width() * _bar_scale);
	if (barWidth < get_height() / 2) barWidth = get_height() / 2;
	return barWidth;
}

int HorBar::slide_width() const
{
	return get_width() - bar_width();
}

int HorBar::bar_x() const
{
	return get_x() + math_ceil(get_value() * slide_width());
}


void HorBar::set_value(float value) { _value = value; }
float HorBar::get_value() const { return _value; }