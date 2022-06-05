#pragma once

#include <Windows.h>
#include <vector>

#include "TileSet.h"
#include "Game.h"

#define TILE_WIDTH_STANDARD 16
#define TILE_HEIGHT_STANDARD 16

class CTileLayer
{
protected:
	int id;
	int width; //Unit: tile
	int height; //Unit: tile

	bool isVisible;
	std::vector<LPTILESET> tileSets;
	int** tileMatrix;

public:
	CTileLayer(int id, int width, int height, bool isVisible = true);

	void AddTileSet(LPTILESET tileSet);
	void GetTileMatrix(int**& tileMatrix) { tileMatrix = this->tileMatrix; };
	void Render();
	void Clear();
	void UnHide() { isVisible = true; }
	int GetId() { return id; }
};

typedef CTileLayer* LPTILELAYER;