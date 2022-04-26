#pragma once

#include <Windows.h>
#include "Game.h"
#include "debug.h"



class CTileSet
{
protected:
	int firstGid;

	int tileWidth;
	int tileHeight;

	int tileCount;
	int columnsCount;


	LPTEXTURE texture;
	D3DX10_SPRITE sprite;
	D3DXMATRIX matScaling;

public:
	CTileSet(int firstGid, int tileWidth, int tileHeight
		, int tileCount, int columnsCount, LPTEXTURE texture)
	{
		this->firstGid = firstGid;
		this->tileWidth = tileWidth;
		this->tileHeight = tileHeight;
		this->tileCount = tileCount;
		this->columnsCount = columnsCount;
		this->texture = texture;
	};

	void Draw(int x, int y, int tileGid) {
		float texWidth = (float)texture->getWidth();
		float texHeight = (float)texture->getHeight();

		// Set the sprite’s shader resource view
		sprite.pTexture = texture->getShaderResourceView();

		sprite.TexCoord.x = ((tileGid - 1) * tileWidth) / texWidth;
		sprite.TexCoord.y = (((tileGid - 1)/columnsCount) * tileHeight) / texHeight;

		sprite.TexSize.x = tileWidth / texWidth;
		sprite.TexSize.y = tileHeight / texHeight;

		sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		sprite.TextureIndex = 0;

		D3DXMatrixScaling(&this->matScaling, (FLOAT)tileWidth, (FLOAT)tileHeight, 1.0f);

		CGame* g = CGame::GetInstance();
		float cx, cy;
		g->GetCamPos(cx, cy);

		cx = (FLOAT)floor(cx);
		cy = (FLOAT)floor(cy);

		D3DXMATRIX matTranslation;

		x = (FLOAT)floor(x);
		y = (FLOAT)floor(y);

		D3DXMatrixTranslation(&matTranslation, x - cx, g->GetBackBufferHeight() - y + cy, 0.1f);
		this->sprite.matWorld = (this->matScaling * matTranslation);

		g->GetSpriteHandler()->DrawSpritesImmediate(&sprite, 1, 0, 0);
	}
};

typedef CTileSet* LPTILESET;

