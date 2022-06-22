#include "TileLayer.h"

CTileLayer::CTileLayer(int id, int width, int height, bool isVisible)
{
	this->id = id;
	this->width = width;
	this->height = height;
	this->tileSets = tileSets;
	this->isVisible = isVisible;
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
	if (!isVisible) return;

	// Only render tiles that will appear inside the camera
	float camX, camY = 0.0f;
	CGame::GetInstance()->GetCamPos(camX, camY);
	int firstVisibleTileIndexX = (int)camX / TILE_WIDTH_STANDARD;
	int firstVisibleTileIndexY = (int)camY / TILE_HEIGHT_STANDARD;

	int maxTilesX = SCREEN_WIDTH / TILE_WIDTH_STANDARD;
	int maxTilesY = SCREEN_HEIGHT / TILE_HEIGHT_STANDARD;

	// Have to calculate i, j against min(...) because (firstVisibleY + maxTilesY) or 
	// (firstVisibleX + maxTilesX) may go past the total amount of tiles inside a tilelayer
	
	//Also take (firstVisible + maxTiles) plus 1 to avoid cropping at the end of the camera
	for (int i = firstVisibleTileIndexY; i < min(height, firstVisibleTileIndexY + maxTilesY +1); i++)
	{
		for (int j = firstVisibleTileIndexX; j < min(width, firstVisibleTileIndexX + maxTilesX + 1); j++)
		{
			int tileGid = tileMatrix[i][j];
			if (tileGid == 0) continue;
			int usedTileSetIndex = 0;

			// Somehow we have to put the "k" var initialization outside the loop or else the
			// vector won't take it as element index
			unsigned int k = 0;
			for (; k < tileSets.size()-1; k++);
			{
				if (tileGid <= tileSets[k]->GetLastGid() && tileGid >= tileSets[k]->GetFirstGid())
					usedTileSetIndex = k;
			}

			tileSets[usedTileSetIndex]->Draw(
				(float) j * TILE_WIDTH_STANDARD ,
				(float) i * TILE_HEIGHT_STANDARD ,
				tileGid);

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