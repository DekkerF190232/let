#include "TextArea.h"
#include <iostream>
#include <utility>
#include <algorithm>
#include "utf8helper.h"

TextArea::TextArea(TTF_Font* font, SDL_Color foreground, SDL_Color background) :
	_max_line_width(0),
	_background(background),
	_foreground(foreground),
	_content(std::vector<std::vector<char>*> { new std::vector<char>() }), // deleted in ~textarea
	_cursor_x(0),
	_cursor_y(0),
	_cursor_x_save(0),
	_cursor_x_save_cursor_y(0),
	_selection_y(-1),
	_selection_x(-1),
	_font(font)
{
}

TextArea::~TextArea()
{
	for (auto& c : this->_content) delete c;
	this->_content.clear();
}

void TextArea::render(SDL_Renderer* renderer)
{
	render_lines(renderer);

	render_cursor(renderer);
}

void TextArea::render_lines(SDL_Renderer* renderer) {

	int posX = get_x();
	int posY = get_y();

	int cursorY = posY;

	int lineHeight = TTF_FontHeight(_font) + 1;

	_max_line_width = 0;

	for (size_t i = 0; i < _content.size(); i++)
	{
		std::vector<char>* line = _content[i];
		std::string lineString(line->begin(), line->end());

		if (lineString.size() == 0)
		{
			posY += lineHeight;
			continue;
		}

		render_line(renderer, lineString, posX, posY);

		posY += lineHeight;
	}

	update_content_size();
}

void TextArea::render_line(SDL_Renderer* renderer, std::string& lineString, int& posX, int& posY)
{
	SDL_Surface* textSurface = TTF_RenderUTF8_LCD(this->_font, lineString.c_str(), _foreground, _background);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_Rect textRect{ posX, posY,textSurface->w, textSurface->h };
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

	if (textSurface->w > _max_line_width) _max_line_width = textSurface->w;

	SDL_DestroyTexture(textTexture);
	SDL_FreeSurface(textSurface);
}

void TextArea::render_cursor(SDL_Renderer* renderer)
{
	std::vector<char>* line = _content[get_cursor_y()];
	std::string beforeCursor(line->begin(), line->begin() + get_cursor_x());
	int w = 0;
	if (get_cursor_x() > 0)
	{
		int h;
		TTF_SizeUTF8(_font, beforeCursor.c_str(), &w, &h);
	}

	int lineHeight = TTF_FontHeight(_font) + 1;

	int posX = get_x();
	int posY = get_y() + lineHeight * get_cursor_y();
	SDL_Rect primaryRect{ posX + w + 1, posY, 2, lineHeight };
	SDL_Rect secondaryRect{ primaryRect.x-1, primaryRect.y-1, primaryRect.w+2, primaryRect.h+2};
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x60);
	SDL_RenderFillRect(renderer, &secondaryRect);
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderFillRect(renderer, &primaryRect);
}

void TextArea::input_text(char* text, bool& canceled)
{
	std::vector<char>* line = _content[get_cursor_y()];

	size_t len = strlen(text);
	line->insert(line->begin() + get_cursor_x(), text, text + len);

	set_cursor_x(get_cursor_x() + len);
}

void TextArea::input_key_down(SDL_Keycode keycode, bool& canceled)
{
	const Uint8 * keys = SDL_GetKeyboardState(NULL);

	bool shiftDown = keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT];

	switch (keycode)
	{
	case SDLK_BACKSPACE:
	{
		std::vector<char>* line = _content[get_cursor_y()];
		if (get_cursor_x() > 0)
		{
			int deleted = ucv_delete_from_next(*line, get_cursor_x());
			set_cursor_x(get_cursor_x() - deleted);
		}
		else if (get_cursor_y() > 0)
		{
			std::vector<char>* lineAbove = _content[get_cursor_y() - 1];
			lineAbove->insert(lineAbove->end(), line->begin(), line->end());
			delete line;
			_content.erase(_content.begin() + get_cursor_y());
			set_cursor_y(get_cursor_y() - 1);
			// important: needs to be after set_cursor_y, to save cursor y for using up/down keys
			// cursor y is saved to know the unicode block count of original line,
			// so curssor x can be set correcly when switching lines.
			set_cursor_x(lineAbove->size());
		}
	}
	break;
	case SDLK_DELETE:
	{
		std::vector<char>* line = _content[get_cursor_y()];
		if (std::cmp_less(get_cursor_x(), line->size()))
		{
			ucv_delete_from_first(*line, get_cursor_x());
		}
		else if (_content.size() == 0)
		{
			_content.erase(_content.begin() + _cursor_y);
			delete line;
			set_cursor_x(0);
		}
		else if (get_cursor_x() == line->size() && std::cmp_less(get_cursor_y(), _content.size() - 1))
		{
			std::vector<char>* lineBelow = _content[get_cursor_y() + 1];
			line->insert(line->end(), lineBelow->begin(), lineBelow->end());
			delete lineBelow;
			_content.erase(_content.begin() + _cursor_y + 1);
			// important: update cursor y save, because the line it was at could have been deleted/shifted
			set_cursor_x(get_cursor_x());
		}
	}
	break;
	case SDLK_RETURN:
	{
		std::vector<char>* line = _content[get_cursor_y()];
		if (get_cursor_y() == _content.size() - 1 && get_cursor_x() == line->size())
		{
			std::vector<char>* newLine = new std::vector<char>(); // deleted in ~textarea
			_content.insert(_content.begin() + get_cursor_y() + 1, newLine);
			set_cursor_y(get_cursor_y() + 1);
			set_cursor_x(0);
		}
		else
		{
			std::vector<char>* newLine = new std::vector<char>(); // deleted in ~textarea
			newLine->insert(newLine->begin(), line->begin() + get_cursor_x(), line->end());
			_content.insert(_content.begin() + get_cursor_y() + 1, newLine);
			line->erase(line->begin() + get_cursor_x(), line->end());
			set_cursor_y(get_cursor_y() + 1);
			set_cursor_x(0);
		}
	}
	break;
	case SDLK_LEFT:
	{
		if (get_cursor_x() > 0)
		{
			std::vector<char>* line = _content[get_cursor_y()];
			set_cursor_x(ucv_dec(*line, get_cursor_x()));
		}
		else if (get_cursor_y() > 0)
		{
			set_cursor_y(get_cursor_y() - 1);
			set_cursor_x(_content[_cursor_y]->size());
		}
	}
	break;
	case SDLK_RIGHT:
	{
		std::vector<char>* line = _content[get_cursor_y()];
		if (std::cmp_less(get_cursor_x(), line->size()))
		{
			set_cursor_x(ucv_inc(*line, get_cursor_x()));
		}
		else if (std::cmp_less(_cursor_y, _content.size() - 1))
		{
			set_cursor_y(get_cursor_y() + 1);
			set_cursor_x(0);
		}
	}
	break;
	case SDLK_UP:
	{
		if (get_cursor_y() > 0)
		{
			set_cursor_y(get_cursor_y() - 1);
			std::vector<char>* line = _content[get_cursor_y()];
			std::vector<char>* save_line = _content[get_cursor_x_save_cursor_y()];
			auto cursorX = static_cast<size_t>(get_cursor_x_save());
			cursorX = ucv_index_to_count(*save_line, cursorX);
			cursorX = ucv_count_to_index(*line, cursorX);
			set_cursor_x_unsaved(cursorX);
		}
		else
		{
			set_cursor_x_unsaved(0);
		}
	}
	break;
	case SDLK_DOWN:
	{
		if (std::cmp_less(get_cursor_y(), _content.size() - 1))
		{
			set_cursor_y(get_cursor_y() + 1);
			std::vector<char>* line = _content[get_cursor_y()];
			std::vector<char>* save_line = _content[get_cursor_x_save_cursor_y()];
			auto cursorX = static_cast<size_t>(get_cursor_x_save());
			cursorX = ucv_index_to_count(*save_line, cursorX);
			cursorX = ucv_count_to_index(*line, cursorX);
			set_cursor_x_unsaved(cursorX);
		}
		else
		{
			std::vector<char>* line = _content[get_cursor_y()];
			set_cursor_x_unsaved(line->size());
		}
	}
	break;
	default:
		break;
	}

	check_cursor();
}

void TextArea::set_selection(int selection_x, int selection_y)
{
	this->_selection_x = selection_x;
	this->_selection_y = selection_y;
}

void TextArea::clear_selection()
{

	this->_selection_x = -1;
	this->_selection_y = -1;
}

void TextArea::set_cursor_x(int cursor_x)
{
	this->_cursor_x = cursor_x;
	this->_cursor_x_save = get_cursor_x();
	this->_cursor_x_save_cursor_y = get_cursor_y();
}

void TextArea::set_cursor_x_unsaved(int cursor_x)
{
	this->_cursor_x = cursor_x;
}

void TextArea::set_cursor_y(int cursor_y)
{
	this->_cursor_y = cursor_y;
}

void TextArea::check_cursor() const
{
	if (get_cursor_y() < 0 || std::cmp_greater_equal(get_cursor_y(), _content.size())) throw std::runtime_error("line out of bounds: " + get_cursor_y());
	std::vector<char>* line = _content[get_cursor_y()];
	if (get_cursor_x() < 0 || std::cmp_greater(get_cursor_x(), line->size())) throw std::runtime_error("column out of bounds: " + get_cursor_y());
}

void TextArea::update_content_size()
{
	int lineHeight = TTF_FontHeight(_font) + 1;
	set_content_size(_max_line_width, _content.size() * lineHeight);
}

bool TextArea::has_selection() const { return this->_selection_x != -1; }
int TextArea::get_cursor_x() const { return _cursor_x; }
int TextArea::get_cursor_x_save() const { return _cursor_x_save; }
int TextArea::get_cursor_x_save_cursor_y() const { return _cursor_x_save_cursor_y; }
int TextArea::get_cursor_y() const { return _cursor_y; }
int TextArea::get_selection_x() const { return _selection_x; }
int TextArea::get_selection_y() const { return _selection_y; }
