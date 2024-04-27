#pragma once

#include "Root.h"

#include "Ribon.h"
#include "FlexElement.h"
#include "Border.h"

class MainRoot : public Root
{

	TTF_Font* _font;
	TTF_Font* _ui_font;

	std::shared_ptr<Ribon> _ribon;

public:
	MainRoot(UiState* state);
	~MainRoot();

};

