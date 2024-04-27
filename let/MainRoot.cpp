#include "MainRoot.h"
#include "TextArea.h"
#include "Border.h"
#include "Flexer.h"
#include "HorBar.h"

#include "ScrollPanel.h"

MainRoot::MainRoot(UiState* state) :
	Root(state),
	_ribon(nullptr),
	_font(nullptr),
	_ui_font(nullptr)
{
	_font = TTF_OpenFont("font.ttf", 14);
	if (!_font) throw std::runtime_error("Failed to load font.");

	_ui_font = TTF_OpenFont("segoeui.ttf", 13);

	content(make<Border>(
		Flexer(state, Flexer::VERTICAL)
		.add(_ribon = make<Ribon>(state, _ui_font), FlexData::fixed(22))
		.add(Flexer(state, Flexer::VERTICAL)
			.add(make<Border>(
				make<ScrollPanel>(
					make<TextArea>(
						_font, color(0xffffffff), color(0x202428ff)
					), state),
				state, color(0x000000ff), color(0x202428ff), Thickness(1), Thickness(4)
			), FlexData::weighted(1))
			.build(), FlexData::weighted(1, Thickness(4)))
		.add(make<Border>(
			nullptr, state, color(0x000000ff), color(0x404040ff), Thickness(1), Thickness(4)
		), FlexData::weighted(1, Thickness(4)))
		.build(), state, color(0x00000000), color(0x202020ff), Thickness(0), Thickness(0))
	);
}

MainRoot::~MainRoot()
{
	TTF_CloseFont(_font);
	TTF_CloseFont(_ui_font);
}

