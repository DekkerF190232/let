#pragma once
#include "Parent.h"
class Dialog : public Parent
{
	bool _hovered;
	bool _closed;
public:
	Dialog(UiState* state);
	void close_later();

    virtual void on_down_outside() {};

    void input_text(char* text, bool& covered) override;
    void input_key_down(SDL_Keycode keycode, bool& covered) override;
    void input_mouse_move(int mouseX, int mouseY, bool& covered) override;
    void input_mouse_left_down(bool& canceled) override;
    void input_mouse_left_up(bool& canceled) override;
    void input_mouse_middle_down(bool& canceled) override;
    void input_mouse_middle_up(bool& canceled) override;
    void input_mouse_right_down(bool& canceled) override;
    void input_mouse_right_up(bool& canceled) override;

    bool is_hovered() const;
    bool is_closed() const;
};

