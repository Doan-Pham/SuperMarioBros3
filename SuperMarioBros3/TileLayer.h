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

	int width; //Unit: tile
	int height; //Unit: tile

	std::vector<LPTILESET> tileSets;
	int** tileMatrix;

public:
	CTileLayer(int id, int width, int height);

	void AddTileSet(LPTILESET tileSet);
	void GetTileMatrix(int**& tileMatrix) { tileMatrix = this->tileMatrix; };
	void Render();
	void Clear();

};

typedef CTileLayer* LPTILELAYER;