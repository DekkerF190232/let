#pragma once
#include "Parent.h"
#include "Thickness.h"
class Border : public Parent
{
	std::shared_ptr<Element> _child;
	SDL_Color _color;
	SDL_Color _background;
	Thickness _thickness;
	Thickness _margin;

public:
	Border
	(
		std::shared_ptr<Element> child,
		UiState* state,
		SDL_Color color,
		SDL_Color background,
		Thickness thickness,
		Thickness margin
	);

	static std::shared_ptr<Border> background
	(
		std::shared_ptr<Element> child,
		SDL_Color color,
		UiState* state
	);

	void render(SDL_Renderer* renderer) override;
	void on_bounds_updated() override;
};

