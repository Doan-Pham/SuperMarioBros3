#pragma once

#include <Windows.h>

#include "Game.h"
#include "debug.h"
#include "Sprite.h"

class CTileSet
{
protected:
	int firstGid;
	int lastGid;

	int tileWidth;
	int tileHeight;

	int tileCount;
	int columnsCount;

	LPTEXTURE texture;
	D3DX10_SPRITE sprite;
	D3DXMATRIX matScaling;

public:
	CTileSet(int firstGid, int tileWidth, int tileHeight
		, int tileCount, int columnsCount, LPTEXTURE texture);

	void GetTileWidthHeight(int& tileWidth, int& tileHeight) 
	{
		tileWidth = this->tileWidth;
		tileHeight = this->tileHeight;
	};

	int GetLastGid() { return lastGid; }

	void Draw(float x, float y, int tileGid);
};

typedef CTileSet* LPTILESET;

