#include "TileMap.h"

CTileMap::CTileMap(int id, LPCWSTR mapFilePath, int width, int height, int tileWidth, int tileHeight)
{
	this->mapFilePath = mapFilePath;
	this->width = width;
	this->height = height;
	this->tileWidth = tileWidth;
	this->tileHeight = tileHeight;
}

void CTileMap::Add(LPTILELAYER layer)
{
	for (unsigned int i = 0; i < tileSets.size(); i++)
		layer->AddTileSet(tileSets[i]);
	tileLayers.push_back(layer);
};

void CTileMap::Render()
{
	for (unsigned int i = 0; i < tileLayers.size(); i++)
		tileLayers[i]->Render();
};

void CTileMap::Clear()
{
	for (unsigned int i = 0; i < tileLayers.size(); i++)
	{
		tileLayers[i]->Clear();
	}
	tileLayers.clear();

	for (unsigned int i = 0; i < tileSets.size(); i++)
	{
		if (tileSets[i] != nullptr) delete tileSets[i];
	}
	tileSets.clear();

	if (mapFilePath != nullptr)
	{
		mapFilePath = NULL;
	}
}