#pragma once
#include "Border.h"
#include "SimpleButton.h"
#include "Item.h"
#include "Flexer.h"
#include "Border.h"
#include "Menu.h"

class Ribon : 
	public Parent,
	public Listener<SimpleButton<Item*>>,
	public Listener<Menu>
{
	TTF_Font* _font;
	std::shared_ptr<Border> _content;
	std::vector<Item*> _items;
	std::vector<std::vector<Item*>> _sub_menu_items;

public:
	Ribon(UiState* state, TTF_Font* font);
	~Ribon();

	void render(SDL_Renderer* renderer) override;
	void on_bounds_updated() override;
	void handle_action(SimpleButton<Item*>* button) override;
	void handle_action(Menu* menu) override;
};

