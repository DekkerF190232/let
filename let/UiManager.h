#pragma once
#include "Root.h"
#include <SDL.h>
#include "UiState.h"

class UiManager
{
private:
	std::unique_ptr<Root> _root;
	int _width;
	int _height;
	UiState _state;
public:
	UiManager();
	~UiManager();

	void on_root_change();
	void resize(int width, int height);

	void render(SDL_Renderer* renderer);
	void input_text(char* text, bool& canceled);
	void input_key_down(SDL_Keycode keycode, bool& canceled);
	void input_mouse_move(int mouse_x, int mouse_y, bool& covered);
	void input_mouse_left_down(bool& canceled);
	void input_mouse_left_up(bool& canceled);
	void input_mouse_middle_down(bool& canceled);
	void input_mouse_middle_up(bool& canceled);
	void input_mouse_right_down(bool& canceled);
	void input_mouse_right_up(bool& canceled);

	void set_root(std::unique_ptr<Root>&& root);
	Root* get_root();
	UiState* get_state();
};

