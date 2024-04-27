#pragma once
#include <vector>
#include "Tile.h"

class GridCell
{
private:
	bool _vertical;
	float _weight;
	Tile* _tile;
	GridCell* _parent;
	static int _idCounter;
	int _id;

public:
	std::vector<GridCell*> children;
	int x;
	int y;
	int width;
	int height;

	GridCell(Tile* tile, GridCell* parent, bool vert);

	void _set_bounds(int x, int y, int width, int height);
	bool is_vertical() const;
	void _set_vertical(bool vertical);
	void set_tile(Tile* tile);
	Tile* get_tile();
	void set_weight(float weight);
	float get_weight() const;
	void _set_parent(GridCell* parent);
	GridCell* get_parent() const;
	int id() const;
};

