#include "Dialog.h"

Dialog::Dialog(UiState* state) :
	Parent(state),
	_hovered(false),
	_closed(false)
{
}

void Dialog::close_later()
{
	if (_closed) return;
	_closed = true;
	auto parent = static_cast<Parent*>(get_parent());
	parent->remove_later(this);
	get_state()->trigger_mouse_move();
}

void Dialog::input_mouse_move(int mouseX, int mouseY, bool& covered)
{
	Parent::input_mouse_move(mouseX, mouseY, covered);

	_hovered = !covered && check_hovered();
	covered = true;
}

void Dialog::input_text(char* text, bool& canceled) { Parent::input_text(text, canceled); canceled = true; }
void Dialog::input_key_down(SDL_Keycode keycode, bool& canceled) { Parent::input_key_down(keycode, canceled); canceled = true; }
void Dialog::input_mouse_left_down(bool& canceled) { Parent::input_mouse_left_down(canceled); if (is_hovered()) canceled = true; else on_down_outside(); }
void Dialog::input_mouse_left_up(bool& canceled) { Parent::input_mouse_left_up(canceled); if (is_hovered()) canceled = true; }
void Dialog::input_mouse_middle_down(bool& canceled) { Parent::input_mouse_middle_down(canceled); if (is_hovered()) canceled = true; else on_down_outside(); }
void Dialog::input_mouse_middle_up(bool& canceled) { Parent::input_mouse_middle_up(canceled); if (is_hovered()) canceled = true; }
void Dialog::input_mouse_right_down(bool& canceled) { Parent::input_mouse_right_down(canceled); if (is_hovered()) canceled = true; else on_down_outside(); }
void Dialog::input_mouse_right_up(bool& canceled) { Parent::input_mouse_right_up(canceled); if (is_hovered()) canceled = true; }
bool Dialog::is_hovered() const { return _hovered; }
bool Dialog::is_closed() const { return _closed; }
