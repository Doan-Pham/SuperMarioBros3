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

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				//int tileGid = tileMatrix[i][j];
				tileSets[0]->Draw(i * 16, j * 16, tileMatrix[i][j]);
			}
		}
	};
};

typedef CTileLayer* LPTILELAYER;