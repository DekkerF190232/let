#include "UiState.h"
#include <limits>
#include <stdexcept>

UiState::UiState() :
	_mouse_x(std::numeric_limits<int>::min()),
	_mouse_y(std::numeric_limits<int>::min()),
	_is_mouse_move_triggered(false),
	_x_off({0}),
	_y_off({0})
{

}

void UiState::trigger_mouse_move()
{
	_is_mouse_move_triggered = true;
}

void UiState::_remove_later(void* parent, void* child)
{
	auto itr = _removes.find(parent);
	if (itr == _removes.end())
	{
		_removes.insert({ parent, std::set<void*>{ child } });
	}
	else
	{
		itr->second.insert(child);
	}
}

void UiState::run_later(std::function<void()> later)
{
	_run_laters.push_back(later);
}

void UiState::open_dialog(void* dialog)
{
	_new_dialogs.push_back(dialog);
	trigger_mouse_move();
}

void UiState::push_off(int x, int y)
{
	_x_off.push(x);
	_y_off.push(y);
}

void UiState::pop_off()
{
	_x_off.pop();
	_y_off.pop();
	if (_y_off.size() == 0) throw new std::runtime_error("offset stack underflow");
}

void UiState::_set_mouse_x(int mouse_x) { this->_mouse_x = mouse_x; }
void UiState::_set_mouse_y(int mouse_y) { this->_mouse_y = mouse_y; }
int UiState::get_mouse_x() const { return _mouse_x; }
int UiState::get_mouse_y() const { return _mouse_y; }
int UiState::x_off() const { return _x_off.top(); }
int UiState::y_off() const { return _y_off.top(); }
int UiState::current_mouse_x() const { return _mouse_x + x_off(); }
int UiState::current_mouse_y() const { return _mouse_y + y_off(); }
