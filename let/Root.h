#pragma once
#include "Parent.h"
#include "Dialog.h"
class Root : public Parent
{

private:
	std::shared_ptr<Parent> _dialogs;
	std::shared_ptr<Element> _content;

protected:
	void content(std::shared_ptr<Element> element);

public:
	Root(UiState* state);

	void _open(std::shared_ptr<Dialog> dialog);

	void on_bounds_updated() override;

};

