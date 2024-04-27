#pragma once
#include "Element.h"
#include "Listener.h"

class Button : public Element
{
	bool _hovered;
	bool _enabled;
	bool _pressed;

public:
	std::function<void()> _on_click;

	Button();
	Button(std::function<void()> on_click);

	void input_mouse_move(int mouse_x, int mouse_y, bool& covered) override;
	void input_mouse_left_down(bool& cancel) override;
	void input_mouse_left_up(bool& cancel) override;

	bool is_enabled() const;
	bool is_hovered() const;
	bool is_pressed() const;
};

