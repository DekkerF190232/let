#pragma once

#include "FlexElement.h"

class Flexer
{

	std::shared_ptr<FlexElement> _element;
	bool _built;

public:

	static const bool VERTICAL = true;
	static const bool HORIZONTAL = false;

	Flexer(UiState* state, bool vertical);

	Flexer add(std::shared_ptr<Element> element, FlexData data);

	std::shared_ptr<FlexElement> build();

};

