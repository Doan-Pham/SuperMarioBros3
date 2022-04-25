#pragma once

#include <Windows.h>
#include <vector>

using namespace std;

struct CTileSet
{
	int id;

	int firstGid;

	LPCWSTR imageSourcePath;

	int tileWidth;
	int tileHeight;

	int tileCount;
	int columnsCount;
};
typedef CTileSet* LPTILESET;

class CTileLayer
{
protected:
	int id;

	int width;
	int height;

	vector<LPTILESET> tileSets;
	UINT** tileMatrix;
public:
	CTileLayer(int id, int width, int height, vector<LPTILESET> tileSets)
	{
		this->id = id;
		this->width = width;
		this->height = height;
		this->tileSets = tileSets;
	}
};

typedef CTileLayer* LPTILELAYER;