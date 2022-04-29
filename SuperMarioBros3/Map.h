#pragma once
#include <Windows.h>
#include <vector>

#include "TileLayer.h"
#include "TileSet.h"

//TODO: Consider adding CTileLayerManager, CTileSetManager, CObjectGroupManager
class CMap
{
protected:
	int id;
	LPCWSTR mapFilePath;
	std::vector<LPTILESET> tileSets;
	std::vector<LPTILELAYER> tileLayers;

public:
	CMap(int id, LPCWSTR mapFilePath)
	{
		this->id = id;
		this->mapFilePath = mapFilePath;
	}

	void Add(LPTILELAYER layer)
	{
		for (int i = 0; i < tileSets.size(); i++)
			layer->AddTileSet(tileSets[i]);
		tileLayers.push_back(layer);
	};
	void Add(LPTILESET tileSet) { tileSets.push_back(tileSet); };

	void Render() 
	{
		for (int i = 0; i < tileLayers.size(); i++)
			tileLayers[i]->Render();
	};
	void Clear() 
	{
		for (int i = 0; i < tileLayers.size(); i++)
		{
			tileLayers[i]->Clear();
		}
		tileLayers.clear();

		for (int i = 0; i < tileSets.size(); i++)
		{
			if (tileSets[i] != nullptr) delete tileSets[i];
		}
		tileSets.clear();

		if (mapFilePath != nullptr)
		{
			mapFilePath = NULL;
		}
	}
};

typedef CMap* LPMAP;