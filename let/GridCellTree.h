#pragma once
#include "GridCell.h"
#include <map>
class GridCellTree
{
private:
	GridCell* _root;
public:
	GridCellTree();
	~GridCellTree();
	void add(GridCell* cell, Side side, Tile* tile);
	void remove(Tile* tile);
	GridCell* find(GridCell* cell, Tile* tile);
	GridCell* get_root() const;
	void print();
private:
	void print(GridCell* cell, size_t level);
};

