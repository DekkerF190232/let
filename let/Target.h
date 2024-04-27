#pragma once
#include "GridCell.h"
#include "Element.h"
class Target :
    public Element
{

    GridCell* _cell;
    Side _side;
    SDL_Color _debugColor;
    bool _hovered;
public:
    Target(GridCell* cell, Side side);
    Target(GridCell* cell, Side side, SDL_Color debugColor);
    void input_mouse_move(int mouseX, int mouseY, bool& covered) override;
    void render(SDL_Renderer* renderer) override;
    bool is_hovered();
    GridCell* get_cell();
    Side get_side();
};

