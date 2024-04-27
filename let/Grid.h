#pragma once
#include "Parent.h"
#include "GridCellTree.h"
#include "Tile.h"
#include "Splitter.h"
#include "Target.h"
#include "Listener.h"

class Grid : public Parent, public Listener<Splitter>
{

private:
	GridCellTree tree;
	std::shared_ptr<Parent> tiles;
	std::shared_ptr<Parent> splitters;
	std::shared_ptr<Parent> targets;

public:

	Grid(UiState* state);
	void render(SDL_Renderer* renderer) override;
	void input_mouse_left_down(bool& canceled) override;
	void on_bounds_updated() override;

	void do_layout(GridCell* cell, 
		int x, int y, int width, int height, 
		int minTgtX, int minTgtY, int maxTgtWidth, int maxTgtHeight);
	void do_layout_add_splitter(GridCell*& child, GridCell* cell, const size_t& i, int layoutMinNewSize, bool vert, int x, int position, int splitterCenterOffset, int width, int layoutSplitterSize, int y, int height);
	void do_layout_add_targets_between(bool vert, GridCell*& child, int inMinTgtX, int position, int targetCenterOffset, int inMaxTgtWidth, int layoutTargetSize, int inMinTgtY, int inMaxTgtHeight);
	void do_layout_add_targets_around(GridCell* cell, int& inMinTgtX, int& inMinTgtY, int& inMaxTgtWidth, int layoutTargetSize, int& inMaxTgtHeight);

	void add(GridCell* cell, Side side, std::shared_ptr<Tile> tile);
	void close(Tile* tile);
	
	std::shared_ptr<Target> find_single_hovered_target();

	void handle_action(Splitter* t) override;

};

