#include "Flexer.h"

Flexer::Flexer(UiState* state, bool vertical) :
    _element(std::make_shared<FlexElement>(state, vertical)),
    _built(false)
{
}

Flexer Flexer::add(std::shared_ptr<Element> element, FlexData data)
{
    if (_built) throw std::runtime_error("flex builder already built");
    _element->_add(element, data);
    return *this;
}

std::shared_ptr<FlexElement> Flexer::build()
{
    if (_built) throw std::runtime_error("flex builder already built");
    _built = true;
    return _element;
}
