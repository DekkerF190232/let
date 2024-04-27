#include "Root.h"

void Root::content(std::shared_ptr<Element> element)
{
	add(_content = element);
}

Root::Root(UiState* state) :
	Parent(state),
	_content(nullptr),
	_dialogs(make<Parent>(state))
{
	add(_dialogs);
}

void Root::_open(std::shared_ptr<Dialog> dialog)
{
	_dialogs->add(dialog);
}

void Root::on_bounds_updated()
{
	if (_content) _content->_layout_update_bounds(get_x(), get_y(), get_width(), get_height());
}
