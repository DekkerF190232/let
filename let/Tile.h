#pragma once
#include "Parent.h"
class Tile :
    public Parent
{
private:
    SDL_Color _color;

public:
    Tile(UiState* state);
    Tile(UiState* state, SDL_Color color);

    void on_bounds_updated() override;
    void render(SDL_Renderer* renderer) override;

};

