#include "UiManager.h"

UiManager::UiManager()
	: _width(0), _height(0), _root(nullptr)
{
}

UiManager::~UiManager()
{
	if (this->_root != nullptr)
	{
		this->_root = nullptr;
	}
}

void UiManager::set_root(std::unique_ptr<Root>&& root)
{
	this->_root = std::move(root);
	on_root_change();
}

Root* UiManager::get_root() { return this->_root.get(); }

UiState* UiManager::get_state() { return &_state; }

void UiManager::render(SDL_Renderer* renderer)
{
	if (this->_root == nullptr) return;
	this->_root->render(renderer);
}

void UiManager::on_root_change()
{
	if (this->_root == nullptr) return;
	this->_root->_layout_update_bounds(0, 0, this->_width, this->_height);
}

void UiManager::resize(int width, int height)
{
	this->_width = width;
	this->_height = height;
	if (this->_root == nullptr) return;
	this->_root->_layout_update_bounds(0, 0, width, height);
}

void UiManager::input_text(char* text, bool& covered)
{
	if (this->_root == nullptr) return;
	this->_root->input_text(text, covered);
}

void UiManager::input_key_down(SDL_Keycode keycode, bool& covered)
{
	if (this->_root == nullptr) return;
	this->_root->input_key_down(keycode, covered);
}

void UiManager::input_mouse_move(int mouse_x, int mouse_y, bool& covered) {
	_state._set_mouse_x(mouse_x);
	_state._set_mouse_y(mouse_y);
	if (this->_root != nullptr)
		this->_root->input_mouse_move(mouse_x, mouse_y, covered);
}

void UiManager::input_mouse_left_down(bool& canceled) { if (this->_root != nullptr) this->_root->input_mouse_left_down(canceled); }
void UiManager::input_mouse_left_up(bool& canceled) { if (this->_root != nullptr) this->_root->input_mouse_left_up(canceled); }
void UiManager::input_mouse_middle_down(bool& canceled) { if (this->_root != nullptr) this->_root->input_mouse_middle_down(canceled); }
void UiManager::input_mouse_middle_up(bool& canceled) { if (this->_root != nullptr) this->_root->input_mouse_middle_up(canceled); }
void UiManager::input_mouse_right_down(bool& canceled) { if (this->_root != nullptr) this->_root->input_mouse_right_down(canceled); }
void UiManager::input_mouse_right_up(bool& canceled) { if (this->_root != nullptr) this->_root->input_mouse_right_up(canceled); }
