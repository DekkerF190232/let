#include "Border.h"

Border::Border(
	std::shared_ptr<Element> child,
	UiState* state,
	SDL_Color color,
	SDL_Color background,
	Thickness thickness,
	Thickness margin
) :
	Parent(state),
	_color(color),
	_background(background),
	_thickness(thickness),
	_child(child),
	_margin(margin)
	// _color({ 0x00,0x00,0x00,0xff }),
	// _background({ 0x00,0x00,0x00,0x00 })
{
	if (child) add(child);
}

std::shared_ptr<Border> Border::background(std::shared_ptr<Element> child, SDL_Color background, UiState* state)
{
	return std::make_shared<Border>(child, state, SDL_Color{ 0 }, background, Thickness { 0 }, Thickness { 0 });
}

void Border::render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, _color.r, _color.g, _color.b, _color.a);

	SDL_Rect top{
		get_x(),
		get_y(),
		get_width(),
		std::min(_thickness.top, get_height())
	};
	SDL_RenderFillRect(renderer, &top);

	SDL_Rect right{
		get_x() + get_width() - _thickness.right,
		get_y() + _thickness.top,
		_thickness.right,
		get_height() - _thickness.ver()
	};
	SDL_RenderFillRect(renderer, &right);

	SDL_Rect bottom{
		get_x(),
		get_y() + get_height() - _thickness.bottom,
		get_width(),
		_thickness.bottom
	};
	SDL_RenderFillRect(renderer, &bottom);

	SDL_Rect left{
		get_x(),
		get_y() + _thickness.top,
		_thickness.left,
		get_height() - _thickness.ver()
	};
	SDL_RenderFillRect(renderer, &left);

	SDL_SetRenderDrawColor(renderer, _background.r, _background.g, _background.b, _background.a);
	SDL_Rect background{
		get_x() + _thickness.left,
		get_y() + _thickness.top,
		get_width() - _thickness.hor(),
		get_height() - _thickness.ver()
	};
	SDL_RenderFillRect(renderer, &background);

	Parent::render(renderer);
}

void Border::on_bounds_updated()
{
	if (_child)
		_child->_layout_update_bounds
		(
			get_x() + _margin.left,
			get_y() + _margin.top,
			std::max(0, get_width() - _margin.hor()),
			std::max(0, get_height() - _margin.ver())
		);
}
