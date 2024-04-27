#include "Grid.h"
#include <stdexcept>
#include <map>

Grid::Grid(UiState* state) :
	Parent(state),
	tiles(std::make_shared<Parent>(state)),
	splitters(std::make_shared<Parent>(state)),
	targets(std::make_shared<Parent>(state))
{
	Parent::add(splitters);
	Parent::add(targets);
	Parent::add(tiles);

	//auto explorer = std::make_shared<Tile>((({ 0xa0, 0xa0, 0xa0, 0xff }));
	auto code = std::make_shared<Tile>(get_state(), SDL_Color{ 0xa0, 0xa0, 0xa0, 0xff });
	//auto console = std::make_shared<Tile>((({ 0xa0, 0xa0, 0xa0, 0xff }));
	//auto props_top = std::make_shared<Tile>((({ 0xa0, 0xa0, 0xa0, 0xff }));
	//auto props_top_right = std::make_shared<Tile>((({ 0xa0, 0xa0, 0xa0, 0xff }));
	//auto props_bottom = std::make_shared<Tile>((({ 0xa0, 0xa0, 0xa0, 0xff }));
	//add(tree.get_root(), Top, explorer);
	add(tree.get_root(), Right, code);
	//add(tree.find(tree.get_root(), code.get()), Bottom, console);
	//add(tree.get_root(), Right, props_bottom);
	//add(tree.find(tree.get_root(), props_bottom.get()), Top, props_top);
	//add(tree.find(tree.get_root(), props_top.get()), Right, props_top_right);
}



void Grid::input_mouse_left_down(bool& canceled)
{
	Parent::input_mouse_left_down(canceled);

	std::shared_ptr<Target> hovered = find_single_hovered_target();
	if (hovered != nullptr)
	{
		add(hovered->get_cell(), hovered->get_side(), std::make_shared<Tile>(get_state(), SDL_Color{ 0xa0, 0xa0, 0xa0, 0xff }));
		
		splitters->clear_later();
		targets->clear_later();

		do_layout(tree.get_root(),
			get_x(), get_y(), get_width(), get_height(),
			get_x(), get_y(), get_width(), get_height());
		get_state()->trigger_mouse_move();
	}
}

void Grid::handle_action(Splitter* splitter)
{
	float newFirstSize = splitter->calc_new_first_size();
	float newSecondSize = splitter->calc_new_second_size();
	
	splitter->first->set_weight(newFirstSize);
	splitter->second->set_weight(newSecondSize);
	
	splitters->clear_later();
	targets->clear_later();

	do_layout(tree.get_root(),
		get_x(), get_y(), get_width(), get_height(),
		get_x(), get_y(), get_width(), get_height());

	get_state()->trigger_mouse_move();
}


void Grid::render(SDL_Renderer* renderer)
{
	Parent::render(renderer);
}


void Grid::on_bounds_updated()
{
	// todo: delete
	splitters->clear_later();
	targets->clear_later();
	do_layout(tree.get_root(),
		get_x(), get_y(), get_width(), get_height(),
		get_x(), get_y(), get_width(), get_height());
}

void Grid::do_layout
(
	GridCell* cell,
	int x, int y, int width, int height,
	int minTgtX, int minTgtY, int maxTgtWidth, int maxTgtHeight
)
{
	int layoutGap = 4;
	int layoutSplitterSize = 6;
	int layoutMinNewSize = 10;

	int layoutTargetSize = std::min(16, std::max(8, width / 20));

	// if size is less than or zero just get the hell out
	if (width <= 0 || height <= 0)
	{
		width = 0;
		height = 0;
		cell->_set_bounds(x, y, 0, 0);
		for (size_t i = 0; i < cell->children.size(); i++)
		{
			GridCell* child = cell->children[i];
			do_layout(child, x, y, 0, 0, x, y, 0, 0);
		}
		return;
	}

	cell->_set_bounds(x, y, width, height);

	int inMinTgtX, inMinTgtY, inMaxTgtWidth, inMaxTgtHeight;
	inMinTgtX = std::max(x, minTgtX);
	inMinTgtY = std::max(y, minTgtY);
	inMaxTgtWidth = std::min(width - (inMinTgtX - x), maxTgtWidth);
	inMaxTgtHeight = std::min(height - (inMinTgtY - y), maxTgtHeight);

	bool canAddTargets = inMaxTgtWidth > layoutTargetSize * 2 && inMaxTgtHeight > layoutTargetSize * 2;

	if (canAddTargets)
	{
		do_layout_add_targets_around(cell, inMinTgtX, inMinTgtY, inMaxTgtWidth, layoutTargetSize, inMaxTgtHeight);
	}

	Tile* tile = cell->get_tile();
	if (tile != nullptr) {
		tile->_layout_update_bounds(x, y, width, height);
		return;
	}

	bool vert = cell->is_vertical();

	float sum = 0;
	for (auto& var : cell->children) sum += var->get_weight();

	int sizeTaken = 0;
	int position = vert ? y : x;

	int splitterCenterOffset = math_round((layoutGap - layoutSplitterSize) / 2.0f);
	int targetCenterOffset = math_round((layoutGap - layoutTargetSize) / 2.0f);

	int available = (vert ? height : width) - (cell->children.size() - 1) * layoutGap;
	if (available < 0) available = 0;
	float lastFloatSize = 0;
	for (size_t i = 0; i < cell->children.size(); i++)
	{
		auto& child = cell->children[i];
		bool last = i + 1 == cell->children.size();
		bool first = i == 0;

		// calculate size
		float scale = child->get_weight() / sum;
		lastFloatSize += scale * available;
		int intSize = static_cast<int>(lastFloatSize);
		lastFloatSize -= intSize;
		int childSize = last ? available - sizeTaken : intSize;
		sizeTaken += childSize;

		// limit child target sizes
		int cMtX = inMinTgtX, cMtY = inMinTgtY, cMtX2 = inMinTgtX + inMaxTgtWidth, cMtY2 = inMinTgtY + inMaxTgtHeight;
		if (vert)
		{
			if (!first) cMtY = position - targetCenterOffset;
			if (!last) cMtY2 = position + childSize + targetCenterOffset;
		}
		else
		{
			if (!first) cMtX = position - targetCenterOffset;
			if (!last) cMtX2 = position + childSize + targetCenterOffset;
		}

		if (vert) do_layout(child, x, position, width, childSize, cMtX, cMtY, cMtX2 - cMtX, cMtY2 - cMtY);
		else do_layout(child, position, y, childSize, height, cMtX, cMtY, cMtX2 - cMtX, cMtY2 - cMtY);

		position += childSize;

		if (!last)
		{
			do_layout_add_splitter(child, cell, i, layoutMinNewSize, vert, x, position, splitterCenterOffset, width, layoutSplitterSize, y, height);
			if (canAddTargets) do_layout_add_targets_between(vert, child, inMinTgtX, position, targetCenterOffset, inMaxTgtWidth, layoutTargetSize, inMinTgtY, inMaxTgtHeight);
		}

		position += layoutGap;
	}
}

void Grid::do_layout_add_splitter(GridCell*& child, GridCell* cell, const size_t& i, int layoutMinNewSize, bool vert, int x, int position, int splitterCenterOffset, int width, int layoutSplitterSize, int y, int height)
{
	auto splitter = std::make_shared<Splitter>(child, cell->children[i + 1], this, layoutMinNewSize);
	if (vert) splitter->_layout_update_bounds(x, position + splitterCenterOffset, width, layoutSplitterSize);
	else splitter->_layout_update_bounds(position + splitterCenterOffset, y, layoutSplitterSize, height);
	splitters->add(splitter);
}

void Grid::do_layout_add_targets_between(bool vert, GridCell*& child, int inMinTgtX, int position, int targetCenterOffset, int inMaxTgtWidth, int layoutTargetSize, int inMinTgtY, int inMaxTgtHeight)
{
	std::shared_ptr<Target> target;
	if (vert)
	{
		target = std::make_shared<Target>(child, Bottom);
		target->_layout_update_bounds(
			inMinTgtX,
			position + targetCenterOffset,
			inMaxTgtWidth,
			layoutTargetSize);
	}
	else
	{
		target = std::make_shared<Target>( child, Right);
		target->_layout_update_bounds(
			position + targetCenterOffset,
			inMinTgtY,
			layoutTargetSize,
			inMaxTgtHeight);
	}
	targets->add(target);
}

void Grid::do_layout_add_targets_around(GridCell* cell, int& inMinTgtX, int& inMinTgtY, int& inMaxTgtWidth, int layoutTargetSize, int& inMaxTgtHeight)
{
	auto parent = cell->get_parent();
	bool topBottom = cell->is_vertical();

	if (parent == nullptr || topBottom)
	{
		auto top = std::make_shared<Target>(cell, Top, SDL_Color { 0x00,0xff,0x00,0xff });
		auto bottom = std::make_shared<Target>(cell, Bottom, SDL_Color{ 0x00,0xff,0x00,0xff });
		top->_layout_update_bounds(inMinTgtX, inMinTgtY, inMaxTgtWidth, layoutTargetSize);
		bottom->_layout_update_bounds(inMinTgtX, inMinTgtY + inMaxTgtHeight - layoutTargetSize, inMaxTgtWidth, layoutTargetSize);
		targets->add(top);
		targets->add(bottom);
		inMinTgtY += layoutTargetSize;
		inMaxTgtHeight -= layoutTargetSize * 2;
	}

	if (parent == nullptr || !topBottom)
	{
		auto left = std::make_shared<Target>(cell, Left, SDL_Color { 0x00,0x00,0xff,0xff });
		auto right = std::make_shared<Target>(cell, Right, SDL_Color { 0x00,0x00,0xff,0xff });
		left->_layout_update_bounds(inMinTgtX, inMinTgtY, layoutTargetSize, inMaxTgtHeight);
		right->_layout_update_bounds(inMinTgtX + inMaxTgtWidth - layoutTargetSize, inMinTgtY, layoutTargetSize, inMaxTgtHeight);
		targets->add(left);
		targets->add(right);
		inMinTgtX += layoutTargetSize;
		inMaxTgtWidth -= layoutTargetSize * 2;
	}
}

void Grid::add(GridCell* cell, Side side, std::shared_ptr<Tile> tile)
{
	//const char* sideNames[] = { "Top", "Right", "Bottom", "Left" };
	//std::cout << "add #" << cell->id() << (cell->is_vertical() ? "V" : "H") << " " << sideNames[side] << std::endl;

	tiles->add(tile);
	tree.add(cell, side, tile.get());

	//std::cout << std::endl;
	//std::cout << "================================================================" << std::endl;
	//tree.print();
	//std::cout << "================================================================" << std::endl;
}

void Grid::close(Tile* tile)
{
}

std::shared_ptr<Target> Grid::find_single_hovered_target()
{
	std::shared_ptr<Target> result = nullptr;
	for (size_t i = 0; i < targets->get_child_count(); i++)
	{
		auto target = std::dynamic_pointer_cast<Target>(targets->get_child(i));
		if (target->is_hovered())
		{
			if (result != nullptr) return nullptr;
			result = target;
		}
	}
	return result;
}
