#pragma once
#include <Windows.h>
#include <unordered_map>

#include "TileSet.h"

#include "debug.h"
using namespace std;

class CTileSetManager
{
	static CTileSetManager* __instance;

	unordered_map<int, LPTILESET> tilesets;

public:
	void Add(int id, LPTILESET tileSet);
	LPTILESET Get(int id);
	void Clear();

	static CTileSetManager* GetInstance();
};

