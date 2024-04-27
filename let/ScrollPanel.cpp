#include "ScrollPanel.h"
#include <memory>

void ScrollPanel::update(const ScrollContent* t)
{
	//std::cout << t->get_content_height() << " " << t->get_content_width() << std::endl;

}

void ScrollPanel::render(SDL_Renderer* renderer)
{
	/*for (size_t i = get_child_count(); i-- > 0;)
	{
		auto child = get_child(i);
		child->render(renderer);
	}*/

	_element->render(renderer);
	_hor_bar->render(renderer);
}

void ScrollPanel::on_bounds_updated()
{
	_flexer->_layout_update_bounds(get_x(), get_y(), get_width(), get_height());
}

void ScrollPanel::on_parent_set()
{
	auto parent = static_cast<Parent*>(get_parent());
	std::weak_ptr<ScrollPanel> weakRef;
	for (size_t i = 0; i < parent->get_child_count(); i++)
	{
		std::shared_ptr<Element> child = parent->get_child(i);
		if (child.get() == this)
		{
			weakRef = dynamic_pointer_cast<ScrollPanel>(child);
			break;
		}
	}
	_content->size_changed.subscribe(weakRef);
}

