#pragma once
#include "Element.h"

#include <vector>
#include <memory>
#include "UiState.h"
class Parent : public Element
{
private:
    std::vector<std::shared_ptr<Element>> _children;

public:
    Parent(UiState* state);

    std::shared_ptr<Element> get_child(int index);
    size_t get_child_count();
    void add(std::shared_ptr<Element> element);
    void remove_later(Element* element);
    void clear_later();
    void focus(Element* element);

    void _remove(Element* element);

    virtual void on_removing(Element* element) {};

    void render(SDL_Renderer* renderer) override;
    void input_text(char* text, bool& canceled) override;
    void input_key_down(SDL_Keycode keycode, bool& canceled) override;
    void input_mouse_move(int mouseX, int mouseY, bool& covered) override;
    void input_mouse_left_down(bool& canceled) override;
    void input_mouse_left_up(bool& canceled) override;
    void input_mouse_middle_down(bool& canceled) override;
    void input_mouse_middle_up(bool& canceled) override;
    void input_mouse_right_down(bool& canceled) override;
    void input_mouse_right_up(bool& canceled) override;

protected:
    const std::vector<std::shared_ptr<Element>>& get_children() const;

};

