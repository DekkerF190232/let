#pragma once
#include "Element.h"
#include "ScrollContent.h"
#include <sstream>
#include <vector>
class TextArea : public Element, public ScrollContent
{
private:
	std::vector<std::vector<char>*> _content;
	int _selection_x;
	int _selection_y;
	int _cursor_x;
	int _cursor_y;
	int _cursor_x_save;
	int _cursor_x_save_cursor_y;
	TTF_Font* _font;
	SDL_Color _foreground;
	SDL_Color _background;
private:
	int _max_line_width;
public:
	TextArea(TTF_Font* font, SDL_Color foreground, SDL_Color background);
	~TextArea();

	void render(SDL_Renderer* renderer) override;
	//void render_background(SDL_Renderer* renderer);
	void render_lines(SDL_Renderer* renderer);
	void render_line(SDL_Renderer* renderer, std::string& lineString, int& posX, int& posY);
	void render_cursor(SDL_Renderer* renderer);

	void input_text(char* text, bool& canceled) override;
	void input_key_down(SDL_Keycode keycode, bool& canceled) override;

	void set_selection(int selection_x, int selection_y);
	void clear_selection();

	void update_content_size();
	bool has_selection() const;
	int get_selection_x() const;
	int get_selection_y() const;
	void set_cursor_x(int cursor_x);
	void set_cursor_x_unsaved(int cursor_x);
	void set_cursor_y(int cursor_y);
	void check_cursor() const;
	int get_cursor_x() const;
	int get_cursor_y() const;
	int get_cursor_x_save() const;
	int get_cursor_x_save_cursor_y() const;
};

