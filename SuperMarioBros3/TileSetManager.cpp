#include "TileSetManager.h"
CTileSetManager* CTileSetManager::__instance = NULL;

CTileSetManager* CTileSetManager::GetInstance()
{
	if (__instance == NULL) __instance = new CTileSetManager();
	return __instance;
}

void CTileSetManager::Add(int id, int firstGid, int tileWidth, int tileHeight
	, int tileCount, int columnsCount, LPTEXTURE texture)
{
	if (tilesets[id] != NULL)
		DebugOut(L"[WARNING] Tileset %d already exists\n", id);

	LPTILESET newTileSet = new CTileSet(
		firstGid, tileWidth, tileHeight, tileCount, columnsCount, texture);

	tilesets[id] = newTileSet;
}

LPTILESET CTileSetManager::Get(int id)
{
	LPTILESET tileSet = tilesets[id];
	if (tileSet == NULL)
		DebugOut(L"[ERROR] Tileset ID %d not found\n", id);
	return tileSet;
}

void CTileSetManager::Clear()
{
	for (auto x : tilesets)
	{
		LPTILESET tileSet = x.second;
		delete tileSet;
	}

	tilesets.clear();
}