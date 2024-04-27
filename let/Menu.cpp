#include "Menu.h"

static int layoutItemHeight = 22;

Menu::Menu(UiState* state, std::vector<Item*> items, int left, int top, Listener<Menu>* listener, TTF_Font* font) :
	Dialog(state),
	_items(items),
	_listener(listener),
	_content(nullptr),
	selected(nullptr)
{

	Flexer buttons(state, Flexer::VERTICAL);
	for (auto& item : _items)
	{
		bool last = item == _items.back();
		buttons.add(make_item_button(item, Thickness(1, 1, last ? 1 : 0, 1), { 6 }, font), FlexData::fixed(layoutItemHeight));
	}

	add(_content = Border::background(
		buttons.build(),
		color(0x404040ff), state)
	);
	_layout_update_bounds(left, top, 100, calculate_height());
}

std::shared_ptr<SimpleButton<Item*>> Menu::make_item_button(Item* item, Thickness border, Thickness margin, TTF_Font* font)
{
	return make<SimpleButton<Item*>>(
		item->text,
		border,
		margin,
		color(0x204060ff),
		color(0x305070ff),
		color(0x103050ff),
		Start,
		Center,
		item,
		this,
		font
	);
}

void Menu::handle_action(SimpleButton<Item*>* t)
{
	selected = t->action;
	if (_listener) _listener->handle_action(this);
	close_later();
}

int Menu::calculate_height() { return _items.size() * layoutItemHeight; }

void Menu::on_down_outside() { close_later(); }

void Menu::on_bounds_updated() { _content->_layout_update_bounds(get_x(), get_y(), get_width(), get_height()); }
