#pragma once
#include "Parent.h"
#include "TextArea.h"
#include "Grid.h"
class TestRoot : public Parent
{
private:
	//TextArea* ta;
	std::shared_ptr<Grid> tm;
	TTF_Font* font;
public:
	TestRoot(UiState* state);
	~TestRoot();
	void on_bounds_updated();
	void render(SDL_Renderer* renderer) override;
};

