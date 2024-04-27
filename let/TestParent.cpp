#include "TestParent.h"

void TestParent::on_bounds_updated()
{
	clear();
	ta = new TextArea();
	ta->_layout_update_bounds(0, 0, get_width(), get_height());
	add(ta);
}
