#include "GridCell.h"

int GridCell::_idCounter = 0;

GridCell::GridCell(Tile* tile, GridCell* parent, bool vert) :
	_vertical(vert),
	_weight(1),
	_tile(tile),
	_parent(parent),
	_id(_idCounter++),
	x(std::numeric_limits<int>::min()),
	y(std::numeric_limits<int>::min()),
	width(0),
	height(0)
{}

void GridCell::set_weight(float weight) { 
	if (weight < 0) throw std::runtime_error("size < 0");
	this->_weight = weight; 
}
void GridCell::_set_bounds(int x, int y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}
bool GridCell::is_vertical() const { return _vertical; }
void GridCell::set_tile(Tile* tile) { this->_tile = tile; }
Tile* GridCell::get_tile() { return _tile; }
float GridCell::get_weight() const { return _weight; }
void GridCell::_set_vertical(bool vertical) { this->_vertical = vertical; }
void GridCell::_set_parent(GridCell* parent) { this->_parent = parent; }
GridCell* GridCell::get_parent() const { return _parent; }
int GridCell::id() const { return _id; }
