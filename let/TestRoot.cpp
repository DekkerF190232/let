#include "TestRoot.h"

TestRoot::TestRoot(UiState *state) :
	Parent(state),
	tm(std::make_shared<Grid>(get_state())),
	font(nullptr)
{
	font = TTF_OpenFont("font.ttf", 14);
	if (!font) throw std::runtime_error("Failed to load font.");

	add(tm);
}

TestRoot::~TestRoot()
{
	TTF_CloseFont(font);
	font = nullptr;
}

void TestRoot::on_bounds_updated()
{
	//ta->_layout_update_bounds(0+2, 0+2, get_width()-4, get_height()-4);
	tm->_layout_update_bounds(0 + 2, 0 + 2, std::max(0, get_width() - 4), std::max(0, get_height() - 4));
}

void TestRoot::render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0x20, 0x24, 0x28, 0xff);
	SDL_Rect rect{ get_x(), get_y(), get_width(), get_height() };
	SDL_RenderFillRect(renderer, &rect);

	Parent::render(renderer);
}
