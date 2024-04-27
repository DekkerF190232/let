#pragma once

#include "Dialog.h"
#include "Flexer.h"
#include "SimpleButton.h"
#include "Border.h"
#include "Item.h"

class Menu : public Dialog, public Listener<SimpleButton<Item*>>
{
	std::shared_ptr<Border> _content;
	std::vector<Item*> _items;
	Listener<Menu>* _listener;

public:
	Item* selected;
	
	Menu(UiState* state, std::vector<Item*> items, int left, int top, Listener<Menu>* listener, TTF_Font* font);


	std::shared_ptr<SimpleButton<Item*>> make_item_button(Item* item, Thickness border, Thickness margin, TTF_Font* font);

	int calculate_height();

	void on_down_outside() override;
	void on_bounds_updated() override;


	void handle_action(SimpleButton<Item*>* t) override;

};

