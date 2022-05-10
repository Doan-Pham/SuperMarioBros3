#include "TileLayer.h"

CTileLayer::CTileLayer(int id, int width, int height)
{
	this->width = id;
	this->width = width;
	this->height = height;
	this->tileSets = tileSets;
	tileMatrix = new int* [height];
	for (int i = 0; i < height; i++) tileMatrix[i] = new int[width];
}

void CTileLayer::AddTileSet(LPTILESET tileSet)
{
	tileSets.push_back(tileSet);
};

void CTileLayer::Render()
{
	//int renderCount = 0;

	float camX, camY = 0.0f;
	CGame::GetInstance()->GetCamPos(camX, camY);
	int firstVisibleTileIndexX = camX / TILE_WIDTH_STANDARD;
	int firstVisibleTileIndexY = camY / TILE_HEIGHT_STANDARD;

	int maxTilesX = SCREEN_WIDTH / TILE_WIDTH_STANDARD;
	int maxTilesY = SCREEN_HEIGHT / TILE_HEIGHT_STANDARD;

	//Have to calculate i, j against min(...) because (i + maxTilesY)/(j + maxTilesX) 
	//may go past the total amount of tiles inside a tilelayer
	
	//Also take (firstVisible + maxTiles) plus 1 to avoid cropping at the end of the camera
	for (int i = firstVisibleTileIndexY; i < min(height, firstVisibleTileIndexY + maxTilesY +1); i++)
	{
		for (int j = firstVisibleTileIndexX; j < min(width, firstVisibleTileIndexX + maxTilesX + 1); j++)
		{
			int tileGid = tileMatrix[i][j];
			int usedTileSetIndex = tileGid / (tileSets[0]->GetLastGid() + 1);

			//Draw the respective tile in the tileset
			tileSets[usedTileSetIndex]->Draw(
				j * TILE_WIDTH_STANDARD,
				i * TILE_HEIGHT_STANDARD,
				tileMatrix[i][j]);

			//renderCount++;
			//DebugOut(L"i = %i, j = %i, tileMatrix[i][j] = %i\n"
			//	, i, j,tileMatrix[i][j]);
		}
	}
	//DebugOut(L"Render count: %d \n", renderCount);
	//DebugOutTitle(L"camX: %0.5f, camY : %0.5f, currentPositionX : %0.5f, currentPositionY : %0.5f",
	//	camX, camY, currentPositionX, currentPositionY);
};

void CTileLayer::Clear()
{
	for (int i = 0; i < height; i++) {
		delete tileMatrix[i];
	}
	delete tileMatrix;
	tileSets.clear();
}