#pragma once

#include <Windows.h>
#include <vector>

#include "TileSet.h"


class CTileLayer
{
protected:

	int width;
	int height;

	std::vector<LPTILESET> tileSets;
	int** tileMatrix;

public:
	CTileLayer(int id, int width, int height)
	{
		this->width = id;
		this->width = width;
		this->height = height;
		this->tileSets = tileSets;
		tileMatrix = new int* [height];
		for (int i = 0; i < height; i++) tileMatrix[i] = new int[width];
	}

	void AddTileSet(LPTILESET tileSet)
	{
		tileSets.push_back(tileSet);
	};

	void GetTileMatrix(int**& tileMatrix) { tileMatrix = this->tileMatrix; };

	void Render() 
	{
		int tileWidth, tileHeight = 0;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				int tileGid = tileMatrix[i][j];
				int usedTileSetIndex = tileGid / (tileSets[0]->GetLastGid() + 1);

				//Get tileset's tile size
				tileSets[usedTileSetIndex]->GetTileWidthHeight(tileWidth, tileHeight);

				//Draw the respective tile in the tileset
				tileSets[usedTileSetIndex]->Draw(j * tileHeight, i * tileWidth, tileMatrix[i][j]);

				//DebugOut(L"i = %i, j = %i, tileMatrix[i][j] = %i\n"
				//	, i, j,tileMatrix[i][j]);
			}
		}
	};

	void Clear()
	{
		for (int i = 0; i < height; i++) {
			delete tileMatrix[i];
		}
		delete tileMatrix;
		tileSets.clear();
	}
};

typedef CTileLayer* LPTILELAYER;