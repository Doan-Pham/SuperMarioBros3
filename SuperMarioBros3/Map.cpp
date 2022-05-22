#include "Map.h"

CMap::CMap(int id, LPCWSTR mapFilePath, int width, int height, int tileWidth, int tileHeight)
{
	this->id = id;
	this->mapFilePath = mapFilePath;
	this->width = width;
	this->height = height;
	this->tileWidth = tileWidth;
	this->tileHeight = tileHeight;
}

void CMap::Add(LPTILELAYER layer)
{
	for (unsigned int i = 0; i < tileSets.size(); i++)
		layer->AddTileSet(tileSets[i]);
	tileLayers.push_back(layer);
};

void CMap::Render()
{
	for (unsigned int i = 0; i < tileLayers.size(); i++)
		tileLayers[i]->Render();
};

void CMap::Clear()
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

bool CMap::IsGameObjectDeleted(const LPGAMEOBJECT& o)
{
	return o == NULL;
}

void CMap::EraseDeletedObjects()
{
	//vector<LPGAMEOBJECT>::iterator it;
	//for (it = objects.begin(); it != objects.end(); it++)
	//{
	//	LPGAMEOBJECT o = *it;
	//	if (o->IsDeleted())
	//	{
	//		delete o;
	//		*it = NULL;
	//	}
	//}
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), IsGameObjectDeleted),
		objects.end());
	DebugOut(L"CMap::EraseDeletedObjects() was called\n");
}
