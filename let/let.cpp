#include <Windows.h>

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "UiManager.h"
#include "TestRoot.h"
#include "MainRoot.h"

using namespace std;

bool init();
void kill();
bool loop();

SDL_Window* window;
SDL_Renderer* renderer;

UiManager uiManager;

int main(int argc, char** args) {
	SetProcessDPIAware();

	if (!init()) {
		system("pause");
		return 1;
	}

	uiManager.resize(640, 480);
	uiManager.set_root(std::make_unique<MainRoot>(uiManager.get_state()));

	while (loop()) {
		SDL_Delay(10);
	}

	kill();
	return 0;
}

bool loop() {

	SDL_Event e;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	while (SDL_PollEvent(&e) != 0) {
		switch (e.type) {
		case SDL_QUIT:
			return false;
		case SDL_TEXTINPUT:
		{
			bool canceled = false;
			uiManager.input_text(e.text.text, canceled);
		}
		break;
		case SDL_KEYDOWN:
		{
			bool canceled = false;
			uiManager.input_key_down(e.key.keysym.sym, canceled);
		}
		break;
		case SDL_MOUSEMOTION:
		{
			bool covered = false;
			uiManager.input_mouse_move(e.motion.x, e.motion.y, covered);
		}
		break;
		case SDL_WINDOWEVENT:
		{
			bool covered;
			switch (e.window.event)
			{
			case SDL_WINDOWEVENT_LEAVE:
				covered = false;
				uiManager.input_mouse_move((std::numeric_limits<int>::min)(), (std::numeric_limits<int>::min)(), covered);
				break;
			case SDL_WINDOWEVENT_RESIZED:
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				if (e.window.data1 > 0 && e.window.data2 > 0)
				{
					uiManager.resize(e.window.data1, e.window.data2);
				}
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_MINIMIZED:
				int width;
				int height;
				SDL_GetWindowSize(window, &width, &height);
				uiManager.resize(width, height);
				break;
			default:
				break;
			}
		}
		break;
		case SDL_MOUSEBUTTONDOWN:
		{
			bool covered = false;
			switch (e.button.button) {
			case SDL_BUTTON_LEFT:
				uiManager.input_mouse_left_down(covered);
				break;
			case SDL_BUTTON_MIDDLE:
				uiManager.input_mouse_middle_down(covered);
				break;
			case SDL_BUTTON_RIGHT:
				uiManager.input_mouse_right_down(covered);
				break;
			}
		}
		break;
		case SDL_MOUSEBUTTONUP:
		{
			bool covered = false;
			switch (e.button.button) {
			case SDL_BUTTON_LEFT:
				uiManager.input_mouse_left_up(covered);
				break;
			case SDL_BUTTON_MIDDLE:
				uiManager.input_mouse_middle_up(covered);
				break;
			case SDL_BUTTON_RIGHT:
				uiManager.input_mouse_right_up(covered);
				break;
			}
		}
		break;
		}
	}

	auto& laters = uiManager.get_state()->_run_laters;
	for (auto it = laters.begin(); it != laters.end(); laters.erase(it), it = laters.begin())
	{
		(*it)();
	}

	auto& removes = uiManager.get_state()->_removes;
	for (auto it = removes.begin(); it != removes.end(); removes.erase(it), it = removes.begin())
	{
		auto parent = static_cast<Parent*>(it->first);
		for (auto& child : it->second)
		{
			auto c = static_cast<Element*>(child);
			parent->_remove(c);
		}
	}

	auto& dialogs = uiManager.get_state()->_new_dialogs;
	for (auto& dialog : dialogs)
	{
		if (uiManager.get_root()) 
			uiManager.get_root()->_open(std::shared_ptr<Dialog>(static_cast<Dialog*>(dialog)));
	}
	dialogs.clear();

	if (uiManager.get_state()->_is_mouse_move_triggered)
	{
		bool covered = false;
		uiManager.input_mouse_move(
			uiManager.get_state()->get_mouse_x(),
			uiManager.get_state()->get_mouse_y(),
			covered
		);
		uiManager.get_state()->_is_mouse_move_triggered = false;
	}

	uiManager.render(renderer);

	SDL_RenderPresent(renderer);

	return true;
}

bool init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error initializing SDL: " << SDL_GetError() << endl;
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) < 0) {
		cout << "Error initializing SDL_image: " << IMG_GetError() << endl;
		return false;
	}

	if (TTF_Init() < 0) {
		cout << "Error intializing SDL_ttf: " << TTF_GetError() << endl;
		return false;
	}

	window = SDL_CreateWindow("Ledt", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (!window) {
		cout << "Error creating window: " << SDL_GetError() << endl;
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		cout << "Error creating renderer: " << SDL_GetError() << endl;
		return false;
	}

	SDL_StartTextInput();

	return true;
}

void kill() {
	SDL_StopTextInput();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = nullptr;
	renderer = nullptr;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}