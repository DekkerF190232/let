#include "Ribon.h"

Ribon::Ribon(UiState* state, TTF_Font* font) :
	Parent(state),
	_content(nullptr),
	_font(font),
	_items(std::vector<Item*>{
		new Item{ "File" },
		new Item{ "View" },
		new Item{ "Settings" }}),
	_sub_menu_items(std::vector<std::vector<Item*>> {
		{ new Item{ "New" }, new Item{ "Open..." }, new Item{ "Save" }, new Item{ "Save As..." } },
		{ new Item{ "vsm" } },
		{ new Item{ "ssm" } }})
{
	Flexer buttons(state, Flexer::HORIZONTAL);
	for (auto& item : _items)
	{
		bool last = item == _items.back();
		buttons.add(make<SimpleButton<Item*>>(
			item->text, Thickness(1, last ? 1 : 0, 1, 1), item, this, font
		), FlexData::fixed(80));
	}

	add(_content = Border::background(buttons.build(), color(0x183838ff), state));
}

Ribon::~Ribon()
{
	for (auto& item : _items) delete item;
	for (auto& menuItems : _sub_menu_items)
	{
		for (auto& item : menuItems) delete item;
	}
}

void Ribon::render(SDL_Renderer* renderer)
{
	Parent::render(renderer);
}

void Ribon::handle_action(SimpleButton<Item*>* button)
{
	size_t index = std::find(_items.begin(), _items.end(), button->action) - _items.begin();
	get_state()->open_dialog(new Menu(get_state(), _sub_menu_items[index], button->get_x(), button->get_bottom(), this, _font));
}

void Ribon::handle_action(Menu* menu)
{
	if (menu->selected == nullptr)
	{
		return;
	}

	std::cout << "action: " << menu->selected << std::endl;
}


void Ribon::on_bounds_updated()
{
	_content->_layout_update_bounds(get_x(), get_y(), get_width(), get_height());
}


