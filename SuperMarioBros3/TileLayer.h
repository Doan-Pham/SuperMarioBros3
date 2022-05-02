#pragma once

#include <Windows.h>
#include <vector>

#include "TileSet.h"
#include "Game.h"

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
		float camX, camY = 0.0f;
		CGame::GetInstance()->GetCamPos(camX, camY);
		int currentPositionX = camX / 16;
		int currentPositionY = camY / 16;
		int maxTilesX = 320 / 16;
		int maxTilesY = 240 / 16;
		for (int i = currentPositionY; i < min(height,i + maxTilesY); i++)
		{
			for (int j = currentPositionX; j < min(width,j + maxTilesX); j++)
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

		DebugOutTitle(L"camX: %0.5f, camY : %0.5f, currentPositionX : %0.5f, currentPositionY : %0.5f",
			camX, camY, currentPositionX, currentPositionY);
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