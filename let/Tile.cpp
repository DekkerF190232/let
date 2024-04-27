#include "Tile.h"
#include <iostream>

Tile::Tile(UiState* state) :
	Parent(state),
	_color({ 0x40,0x40,0x40,0xff })
{
}

Tile::Tile(UiState* state, SDL_Color color) :
	Parent(state),
	_color(color)
{
}

void Tile::on_bounds_updated()
{
}

void Tile::render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, _color.r, _color.g, _color.b, _color.a);
	SDL_Rect fillRect{ get_x(), get_y(), get_width(), get_height() };
	SDL_RenderFillRect(renderer, &fillRect);

	SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, _color.a);
	SDL_Rect rect{ get_x(), get_y(), get_width(), get_height() };
	SDL_RenderDrawRect(renderer, &rect);
}
