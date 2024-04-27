#pragma once

#include "Button.h"

template<class T>
class SimpleButton : public Button
{

private:
	TTF_Font* _font;
	std::string _text;
	SDL_Color _normal;
	SDL_Color _hovered;
	SDL_Color _pressed;
	Thickness _border;
	Thickness _margin;

	Alignment _align_hor;
	Alignment _align_ver;

public:
	const T action;
	Listener<SimpleButton<T>>* listener;

	SimpleButton(
		std::string text,
		Thickness border,
		T action,
		Listener<SimpleButton<T>>* listener,
		TTF_Font* font
	);

	SimpleButton(
		std::string text,
		Thickness border,
		Thickness margin,
		SDL_Color _normal,
		SDL_Color _hovered,
		SDL_Color _pressed,
		Alignment alignHor,
		Alignment alignVer,
		T action,
		Listener<SimpleButton<T>>* listener,
		TTF_Font* font
	);

	void render(SDL_Renderer* renderer) override;
	void on_click();
};

template<class T>
SimpleButton<T>::SimpleButton
(
	std::string text,
	Thickness border,
	T action,
	Listener<SimpleButton<T>>* listener,
	TTF_Font* font
) :
	Button(std::bind(&SimpleButton::on_click, this)),
	_font(font),
	_text(text),
	_normal(color(0x204040ff)),
	_hovered(color(0x305050ff)),
	_pressed(color(0x103030ff)),
	_border(border),
	_align_hor(Center),
	_align_ver(Center),
	action(action),
	listener(listener),
	_margin(Thickness(0))
{
}

template<class T>
SimpleButton<T>::SimpleButton
(
	std::string text,
	Thickness border,
	Thickness margin,
	SDL_Color normal,
	SDL_Color hovered,
	SDL_Color pressed,
	Alignment alignHor,
	Alignment alignVer,
	T action,
	Listener<SimpleButton<T>>* listener,
	TTF_Font* font
) :
	Button(std::bind(&SimpleButton::on_click, this)),
	_font(font),
	_text(text),
	_normal(normal),
	_hovered(hovered),
	_pressed(pressed),
	_border(border),
	_margin(margin),
	_align_hor(alignHor),
	_align_ver(alignVer),
	action(action),
	listener(listener)
{

}

template<class T>
void SimpleButton<T>::render(SDL_Renderer* renderer)
{
	auto foreground = color(0xffffffff);
	auto& background = is_pressed() ? _pressed : (is_hovered() ? _hovered : _normal);
	render_set_color(renderer, background);

	SDL_Rect rect{
		get_x(),
		Element::get_y(),
		Element::get_width(),
		Element::get_height()
	};
	SDL_RenderFillRect(renderer, &rect);

	int fontHeight = TTF_FontHeight(_font) + 1;
	SDL_Surface* textSurface = TTF_RenderUTF8_LCD(this->_font, _text.c_str(), foreground, background);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_Rect textRect{
		//get_x() + (get_width() - textSurface->w) / 2,
		//get_y() + (get_height() - fontHeight) / 2,
		align(get_x() + _margin.left, get_width() - _margin.hor(), textSurface->w, _align_hor),
		align(get_y() + _margin.top, get_height() - _margin.ver(), fontHeight, _align_ver),
		textSurface->w, textSurface->h };
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

	SDL_DestroyTexture(textTexture);
	SDL_FreeSurface(textSurface);

	render_set_color(renderer, color(0x00000040));
	render_draw_outline(renderer, get_x(), get_y(), get_width(), get_height(), _border);
}

template<class T>
void SimpleButton<T>::on_click()
{
	if (!listener) return;
	listener->handle_action(this);
	// continue here, then add listener to constructor in ribon
}
