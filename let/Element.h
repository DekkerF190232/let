#pragma once

#include <iostream>
#include <limits>

#include <SDL.h>
#include <SDL_ttf.h>

#include "UiState.h"
#include "Side.h"
#include "Thickness.h"
#include "Alignment.h"

class Element
{
private:
	int _x;
	int _y;
	int _width;
	int _height;
	Element* _parent;
	UiState* _state;
public:
	Element();

	void _set_state(UiState* state);
	void _layout_update_bounds(int x, int y, int width, int height);
	void _parent_set(Element* parent);

	virtual void render(SDL_Renderer* renderer) = 0;
	virtual void on_bounds_updated() {}
	virtual void on_parent_set() {}
	virtual void input_text(char* text, bool& canceled) {}
	virtual void input_key_down(SDL_Keycode keycode, bool& canceled) {}
	virtual void input_mouse_move(int mouseX, int mouseY, bool& covered) {}
	virtual void input_mouse_left_down(bool& canceled) {}
	virtual void input_mouse_left_up(bool& canceled) {}
	virtual void input_mouse_middle_down(bool& canceled) {}
	virtual void input_mouse_middle_up(bool& canceled) {}
	virtual void input_mouse_right_down(bool& canceled) {}
	virtual void input_mouse_right_up(bool& canceled) {}

	bool check_hovered();

	int get_x() const;
	int get_y() const;
	int get_width() const;
	int get_height() const;
	int get_right() const;
	int get_bottom() const;
	int current_x() const;
	int current_y() const;
	int current_right() const;
	int current_bottom() const;
	Element* get_parent() const;
	UiState* get_state() const;

	template <class T, class... _Types>
	static std::shared_ptr<T> make(_Types&&... args);

	static float to_float(int i);
	static int math_round(float x);
	static int math_floor(float x);
	static int math_ceil(float x);
	static SDL_Color color(unsigned int rgba);
	static void render_draw_outline(SDL_Renderer* renderer, int x, int y, int width, int height, Thickness thickness);
	static void render_set_color(SDL_Renderer* renderer, SDL_Color color);
	static int align(int pos, int containerSize, int contentSize, Alignment alignment);

};

template<class T, class ..._Types>
inline std::shared_ptr<T> Element::make(_Types && ...args)
{
	return std::make_shared<T>(args...);
}
