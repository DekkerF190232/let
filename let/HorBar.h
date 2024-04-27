#pragma once
#include "Element.h"
class HorBar : public Element
{
	float _bar_scale;
	float _value;
	int _grab_x;
	float _grab_value;

private:
	bool _grabbed;
	bool _bar_hovered;

public:
	HorBar(float value, float barScale);

	void on_bounds_updated() override;
	void render(SDL_Renderer* renderer) override;
	void input_mouse_move(int mouseX, int mouseY, bool& covered) override;
	void input_mouse_left_down(bool& canceled) override;
	void input_mouse_left_up(bool& canceled) override;

	void update_bar_scale(float barScale);
	void set_value(float value);
	float get_value() const;
	int bar_width() const;
	int slide_width() const;
	int bar_x() const;
};

