#pragma once
#include <vector>
#include <functional>
#include <map>
#include <set>
#include <memory>
#include <stack>
class UiState
{
private:
	int _mouse_x;
	int _mouse_y;

	std::stack<int> _x_off;
	std::stack<int> _y_off;

public:
	bool _is_mouse_move_triggered;
	std::vector<std::function<void()>> _run_laters;
	std::map<void*, std::set<void*>> _removes;
	std::vector<void*> _new_dialogs;

	UiState();
	void trigger_mouse_move();
	void run_later(std::function<void()> later);
	void open_dialog(void* dialog);
	void push_off(int x, int y);
	void pop_off();

	void _remove_later(void* parent, void* child);

	void _set_mouse_x(int mouse_x);
	void _set_mouse_y(int mouse_y);
	int get_mouse_x() const;
	int get_mouse_y() const;
	int x_off() const;
	int y_off() const;
	int current_mouse_x() const;
	int current_mouse_y() const;
};

