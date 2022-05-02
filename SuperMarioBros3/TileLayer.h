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
		int firstVisibleTileIndexX = camX / TILE_WIDTH_STANDARD;
		int firstVisibleTileIndexY = camY / TILE_HEIGHT_STANDARD;

		int maxTilesX = SCREEN_WIDTH / TILE_WIDTH_STANDARD;
		int maxTilesY = SCREEN_HEIGHT / TILE_HEIGHT_STANDARD;

		//Have to calculate i, j against min(...) because (i + maxTilesY)/(j + maxTilesX) 
		//may go past the total amount of tiles inside a tilelayer
		for (int i = firstVisibleTileIndexY; i < min(height,i + maxTilesY); i++)
		{
			for (int j = firstVisibleTileIndexX; j < min(width,j + maxTilesX); j++)
			{
				int tileGid = tileMatrix[i][j];
				int usedTileSetIndex = tileGid / (tileSets[0]->GetLastGid() + 1);

				//Draw the respective tile in the tileset
				tileSets[usedTileSetIndex]->Draw(
					j * TILE_WIDTH_STANDARD,
					i * TILE_HEIGHT_STANDARD ,
					tileMatrix[i][j]);

				//DebugOut(L"i = %i, j = %i, tileMatrix[i][j] = %i\n"
				//	, i, j,tileMatrix[i][j]);
			}
		}

		//DebugOutTitle(L"camX: %0.5f, camY : %0.5f, currentPositionX : %0.5f, currentPositionY : %0.5f",
		//	camX, camY, currentPositionX, currentPositionY);
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