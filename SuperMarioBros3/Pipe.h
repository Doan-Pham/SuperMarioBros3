#pragma once
#include "GameObject.h"
#include "debug.h"

#include "Sprites.h"

#define ID_SPRITE_PIPE_HEAD_VERT_UPSIDE		33111
#define ID_SPRITE_PIPE_BODY_VERT			33131

#define PIPE_DIRECTION_VERTICAL_UPSIDE		-1
#define PIPE_DIRECTION_VERTICAL_DOWNSIDE	1
#define PIPE_DIRECTION_HORIZONTAL			2

class CPipe : public CGameObject
{
protected:
	int height; // Unit: cell
	int	width; // Unit: cell
	float cellWidth;
	float cellHeight;
	int direction; // -1: vertical + upside, 1: vertical + downside, 2: horizontal
	bool isMarioSpawnLocation; // Decides if mario can spawn from here

	bool isBlocking;

public:
	CPipe(float x, float y, int width, int height, float cellWidth, float cellHeight,
		int direction,bool isMarioSpawnLocation) :CGameObject(x, y)
	{
		this->width = width;
		this->height = height;
		this->cellWidth = cellWidth;
		this->cellHeight = cellHeight;
		this->direction = direction;
		this->isMarioSpawnLocation = isMarioSpawnLocation;
		isBlocking = true;
	}
	virtual void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	bool IsMarioSpawnLocation() { return isMarioSpawnLocation; }
	int GetDirection() { return direction; }
	int IsBlocking() { return isBlocking; }
	void SetBlocking(bool isBlocking) { this->isBlocking = isBlocking; }
};
