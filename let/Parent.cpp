#include "Parent.h"
#include <stdexcept>

Parent::Parent(UiState* state)
{
	_set_state(state);
}

void Parent::clear_later()
{
	for (auto& el : _children)
	{
		get_state()->_remove_later(this, el.get());
	}
}

void Parent::remove_later(Element* element)
{
	get_state()->_remove_later(this, element);
}

std::shared_ptr<Element> Parent::get_child(int index)
{
	return this->_children[index];
}

size_t Parent::get_child_count()
{
	return this->_children.size();
}

void Parent::add(std::shared_ptr<Element> element)
{
	if (element.get() == nullptr) throw std::invalid_argument("element is null");
	this->_children.push_back(element);
	element->_parent_set(this);
}

void Parent::focus(Element* element)
{
	auto itr = std::find_if(_children.begin(), _children.end(),
		[element](const auto& ptr)
		{
			return ptr.get() == element;
		});
	if (itr == _children.end()) throw std::invalid_argument("element is not in this parent");
	std::shared_ptr<Element> old = *itr;
	_children.erase(itr);
	_children.insert(_children.begin(), old);
}

void Parent::_remove(Element* element)
{
	auto itr = std::find_if(_children.begin(), _children.end(),
		[element](const std::shared_ptr<Element>& ptr)
		{
			return ptr.get() == element;
		});
	if (itr == _children.end()) throw std::invalid_argument("element is not in this parent");
	on_removing(itr->get());
	this->_children.erase(itr);
}

void Parent::render(SDL_Renderer* renderer)
{
	for (auto it = _children.rbegin(); it != _children.rend(); ++it)
	{
		(*it)->render(renderer);
	}
}

void Parent::input_text(char* text, bool& canceled)
{
	for (auto& element : _children)
	{
		element->input_text(text, canceled);
	}
}
void Parent::input_key_down(SDL_Keycode keycode, bool& canceled)
{
	for (auto& element : _children)
	{
		element->input_key_down(keycode, canceled);
	}
}
void Parent::input_mouse_move(int mouse_x, int mouse_y, bool& covered) {
	for (auto& element : _children)
	{
		element->input_mouse_move(mouse_x, mouse_y, covered);
	}
}
void Parent::input_mouse_left_down(bool& canceled) {
	for (auto& element : _children)
	{
		element->input_mouse_left_down(canceled);
	}
}
void Parent::input_mouse_left_up(bool& canceled) {
	for (auto& element : _children)
	{
		element->input_mouse_left_up(canceled);
	}
}
void Parent::input_mouse_middle_down(bool& canceled) {
	for (auto& element : _children)
	{
		element->input_mouse_middle_down(canceled);
	}
}
void Parent::input_mouse_middle_up(bool& canceled) {
	for (auto& element : _children)
	{
		element->input_mouse_middle_up(canceled);
	}
}
void Parent::input_mouse_right_down(bool& canceled) {
	for (auto& element : _children)
	{
		element->input_mouse_right_down(canceled);
	}
}
void Parent::input_mouse_right_up(bool& canceled) {
	for (auto& element : _children)
	{
		element->input_mouse_right_up(canceled);
	}
}

const std::vector<std::shared_ptr<Element>>& Parent::get_children() const
{
	return _children;
}

