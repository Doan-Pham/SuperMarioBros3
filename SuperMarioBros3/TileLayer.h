#pragma once

#include <Windows.h>
#include <vector>

#include "TileSet.h"
#include "MapLayer.h"

using namespace std;

class CTileLayer : public CMapLayer
{
protected:

	int width;
	int height;

	vector<LPTILESET> tileSets;
	int** tileMatrix;
public:
	CTileLayer(int id, int width, int height):
		CMapLayer(id)
	{
		this->width = width;
		this->height = height;
		this->tileSets = tileSets;
		tileMatrix = new int * [height];
		for (int i = 0; i < height; i++) tileMatrix[i] = new int[width];
	}
	void AddTileSet(LPTILESET tileSet) 
	{
		tileSets.push_back(tileSet);
	};
	void GetTileMatrix(int**& tileMatrix) { tileMatrix = this->tileMatrix; };
	void Render() {
		int tileWidth, tileHeight = 0;

		for (int i = 0; i < height;i++)//height; i++)
		{
			for (int j = 0; j < width;j++)//width; j++)
			{
				int tileGid = tileMatrix[i][j];
				tileSets[tileGid / (tileSets[0]->GetLastGid() + 1)]->GetTileWidthHeight(tileWidth, tileHeight);
				tileSets[tileGid / (tileSets[0]->GetLastGid() + 1)]->Draw(j * tileHeight, i * tileWidth, tileMatrix[i][j]);
				//DebugOut(L"i = %i, j = %i, tileMatrix[i][j] = %i\n"
				//	, i, j,tileMatrix[i][j]);
			}
		}
	};

	void Clear() {
		for (int i = 0; i < height; i++) {
			delete tileMatrix[i];
		}
		delete tileMatrix;
		tileSets.clear();
	}
};

typedef CTileLayer* LPTILELAYER;