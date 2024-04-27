#pragma once
#include "Element.h"
#include "GridCell.h"
#include "Listener.h"
class Splitter : public Element
{
private:
	int _min_new_size_px;
	Listener<Splitter>* _completeListener;
	bool _hovered;
	bool _vertical;
	int drag_off_x;
	int drag_off_y;

public:
	GridCell* first;
	GridCell* second;
	int down_x;
	int down_y;

	Splitter(GridCell* first, GridCell* second, Listener<Splitter>* listener, int minNewSizePx);

	void render(SDL_Renderer* renderer) override;
	void input_mouse_move(int mouse_x, int mouse_y, bool& covered) override;
	void input_mouse_left_down(bool &cancel) override;
	void input_mouse_left_up(bool& cancel) override;

	int calc_new_first_size_px() const;
	float calc_new_first_size() const;
	float calc_new_second_size() const;

	bool is_dragging() const;
private:
	int get_size(GridCell* cell) const;
	float get_weight(GridCell* cell) const;
};

