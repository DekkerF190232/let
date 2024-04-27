#include "Target.h"

Target::Target(GridCell* cell, Side side) :
	Target(cell, side, { 0x00,0x00,0x00,0xff })
{
}

Target::Target(GridCell* cell, Side side, SDL_Color color) :
	_side(side),
	_cell(cell),
	_debugColor(color),
	_hovered(false)
{
}

void Target::input_mouse_move(int mouseX, int mouseY, bool& covered)
{
	_hovered = !covered && Element::check_hovered();
}

bool Target::is_hovered() { return _hovered; }
GridCell* Target::get_cell() { return _cell; }
Side Target::get_side() { return _side; }

void Target::render(SDL_Renderer* renderer)
{
	bool debug = true;
	if (debug)
	{
		SDL_SetRenderDrawColor(renderer, _debugColor.r, _debugColor.g, _debugColor.b, is_hovered() ? 0x80 : 0x40);
		SDL_Rect rect{
			get_x(),
			get_y(),
			get_width(),
			get_height()
		};
		SDL_RenderFillRect(renderer, &rect);
	}
}