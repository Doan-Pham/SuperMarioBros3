#pragma once

#include <Windows.h>

class CTileSet
{
protected:
	int firstGid;

	int tileWidth;
	int tileHeight;

	int tileCount;
	int columnsCount;

	LPCWSTR imageSourcePath;

public:
	CTileSet(int firstGid, int tileWidth, int tileHeight
		, int tileCount, int columnsCount, LPCWSTR imageSourcePath)
	{
		this->firstGid = firstGid;
		this->tileWidth = tileWidth;
		this->tileHeight = tileHeight;
		this->tileCount = tileCount;
		this->columnsCount = columnsCount;
		this->imageSourcePath = imageSourcePath;
	};
};

typedef CTileSet* LPTILESET;

