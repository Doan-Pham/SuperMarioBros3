#include "TileSet.h"

CTileSet::CTileSet(int firstGid, int tileWidth, int tileHeight
	, int tileCount, int columnsCount, LPTEXTURE texture)
{
	this->firstGid = firstGid;
	this->tileWidth = tileWidth;
	this->tileHeight = tileHeight;
	this->tileCount = tileCount;
	this->columnsCount = columnsCount;
	this->lastGid = tileCount + firstGid - 1;
	this->texture = texture;
};

void CTileSet::Draw(int x, int y, int tileGid)
{
	int left = ((tileGid - firstGid) % columnsCount) * tileWidth;
	int top = ((tileGid - firstGid) / columnsCount) * tileHeight;
	int right = left + tileWidth - 1;
	int bottom = top + tileHeight - 1;

	LPSPRITE spriteToDraw = new CSprite(0, left, top, right, bottom, texture);
	spriteToDraw->Draw(x, y);
	delete spriteToDraw;
}
;