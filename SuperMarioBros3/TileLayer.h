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

		for (int i = 0; i < 39;i++)//height; i++)
		{
			for (int j = 0; j < 100;j++)//width; j++)
			{
				//int tileGid = tileMatrix[i][j];
				tileSets[0]->Draw(j * 16, i * 16, tileMatrix[i][j]);
				//DebugOut(L"i = %i, j = %i, tileMatrix[i][j] = %i\n"
				//	, i, j,tileMatrix[i][j]);
			}
		}
	};
};

typedef CTileLayer* LPTILELAYER;