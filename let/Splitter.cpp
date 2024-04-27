#include "Splitter.h"

Splitter::Splitter(GridCell* first, GridCell* second, Listener<Splitter>* listener, int minNewSize) :
	_vertical(first->get_parent()->is_vertical()),
	_hovered(false),
	first(first),
	second(second),
	down_x(std::numeric_limits<int>::min()),
	down_y(std::numeric_limits<int>::min()),
	_completeListener(listener),
	drag_off_x(0),
	drag_off_y(0),
	_min_new_size_px(minNewSize)
{
}

void Splitter::render(SDL_Renderer* renderer)
{
	if (is_dragging())
	{
		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
		SDL_Rect rect{
			_vertical ? get_x() : get_x() + calc_new_first_size_px() - first->width,
			_vertical ? get_y() + calc_new_first_size_px() - first->height : get_y(),
			get_width(),
			get_height()
		};
		SDL_RenderFillRect(renderer, &rect);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, _hovered ? 0xa0 : 0x60);
		SDL_Rect rect{
			get_x(),
			get_y(),
			get_width(),
			get_height()
		};
		SDL_RenderFillRect(renderer, &rect);
	}
}

void Splitter::input_mouse_move(int mouse_x, int mouse_y, bool& covered)
{
	_hovered = !covered && Element::check_hovered();
	if (is_dragging() || _hovered) covered = true;
}

void Splitter::input_mouse_left_down(bool& cancel)
{
	if (!_hovered) return;
	if (cancel) return;

	cancel = true;
	down_x = get_state()->get_mouse_x();
	down_y = get_state()->get_mouse_y();
	drag_off_x = down_x - get_x();
	drag_off_y = down_y - get_y();

	get_state()->run_later([this]()
		{
			auto parent = static_cast<Parent*>(get_parent());
			parent->focus(this);
		});
}

void Splitter::input_mouse_left_up(bool& cancel)
{
	if (cancel) return;
	if (!is_dragging()) return;
	cancel = true;
	get_state()->run_later([this]()
		{
			this->_completeListener->handle_action(this);
			down_x = std::numeric_limits<int>::min();
			down_y = std::numeric_limits<int>::min();
		});
}

int Splitter::calc_new_first_size_px() const
{
	int firstSize = get_size(first), secondSize = get_size(second);
	int totalSize = firstSize + secondSize;
	
	int additionalSize = _vertical ?
		get_state()->get_mouse_y() - down_y :
		get_state()->get_mouse_x() - down_x;
	int newFirstSize = firstSize + additionalSize;

	if (totalSize < _min_new_size_px * 2) return firstSize;

	if (newFirstSize < _min_new_size_px) return _min_new_size_px;
	if (newFirstSize > totalSize - _min_new_size_px) return totalSize - _min_new_size_px;

	return newFirstSize;
}


float Splitter::calc_new_first_size() const
{
	int newFirstSize = calc_new_first_size_px();
	float scaleFactor = to_float(get_size(first)) / get_weight(first);

	float firstWeight = to_float(newFirstSize) / scaleFactor;
	if (firstWeight < 0) return get_weight(first);
	float totalWeight = (get_weight(first) + get_weight(second));
	if (firstWeight > totalWeight) return get_weight(first);

	return firstWeight;
}

float Splitter::calc_new_second_size() const { return (get_weight(first) + get_weight(second)) - calc_new_first_size(); }

int Splitter::get_size(GridCell* cell) const { return _vertical ? cell->height : cell->width; }
float Splitter::get_weight(GridCell* cell) const { return  cell->get_weight(); }

bool Splitter::is_dragging() const { return down_x != std::numeric_limits<int>::min(); }
