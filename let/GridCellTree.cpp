#include "GridCellTree.h"
#include <stdexcept>
#include <stack>

GridCellTree::GridCellTree()
{
	_root = new GridCell(nullptr, nullptr, false); // TODO: DELETE
}



GridCellTree::~GridCellTree()
{

}

void GridCellTree::add(GridCell* cell, Side side, Tile* tile)
{
	// set root, if tree is empty
	if (cell == _root && _root->get_tile() == nullptr && _root->children.size() == 0)
	{
		_root->set_tile(tile);
		return;
	}

	bool vert = side == Top || side == Bottom;
	bool first = side == Top || side == Left;
	auto& children = cell->children;
	auto parent = cell->get_parent();

	// insert into parent, if parent has desired orientation
	if (parent != nullptr && parent->is_vertical() == vert)
	{
		auto cellItr = std::find(parent->children.begin(), parent->children.end(), cell);
		auto newChild = new GridCell(tile, parent, !parent->is_vertical());
		if (first) parent->children.insert(cellItr, newChild);
		else parent->children.insert(cellItr + 1, newChild);
		return;
	}

	// split node and set orientation if it is a leaf (has no children)
	if (cell->get_tile() != nullptr) {
		if (cell == _root)
		{
			cell->_set_vertical(vert);
		}

		auto newChild = new GridCell(tile, cell, !cell->is_vertical());
		auto oldChild = new GridCell(cell->get_tile(), cell, !cell->is_vertical());
		cell->set_tile(nullptr);

		if (first) children.push_back(newChild);
		children.push_back(oldChild);
		if (!first) children.push_back(newChild);

		return;
	}

	// 
	if (cell->is_vertical() == vert) {
		auto newChild = new GridCell(tile, cell, !cell->is_vertical());
		if (first) children.insert(children.begin(), newChild);
		else children.push_back(newChild);
		return;
	}

	// cell must be root, bc it has no parent with the desired orientation 
	// and does not have the desired orientation itself
	if (cell == _root)
	{
		auto newRoot = new GridCell(nullptr, nullptr, vert);
		auto oldRoot = _root;
		oldRoot->_set_parent(newRoot);
		oldRoot->_set_vertical(!newRoot->is_vertical());
		_root = newRoot;

		auto newChild = new GridCell(tile, newRoot, !newRoot->is_vertical());
		if (first) newRoot->children.push_back(newChild);
		newRoot->children.push_back(oldRoot);
		if (!first) newRoot->children.push_back(newChild);

		return;
	}

	//throw std::runtime_error("Unsupported cell insert");
}

// TODO: Implement
void GridCellTree::remove(Tile* tile)
{
}

GridCell* GridCellTree::find(GridCell* cell, Tile* tile)
{
	if (cell->get_tile() == tile) return cell;
	for (auto& child : cell->children)
	{
		//if (child->get_parent() != cell) throw std::runtime_error(std::string("bro wth man"));
		auto res = find(child, tile);
		if (res != nullptr) return res;
	}
	return nullptr;
}

GridCell* GridCellTree::get_root() const { return _root; }

void GridCellTree::print() { print(_root, 0); }

void GridCellTree::print(GridCell* cell, size_t level)
{
	for (size_t i = 0; i < level; i++) std::cout << "  ";
	std::cout << "#" << cell->id() << (cell->is_vertical() ? "V" : "H") << std::endl;

	for (size_t i = 0; i < cell->children.size(); i++) print(cell->children[i], level + 1);
}
