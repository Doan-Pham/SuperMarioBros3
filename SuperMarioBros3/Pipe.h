#pragma once
#include "GameObject.h"
#include "debug.h"

#include "Sprites.h"

#define ID_SPRITE_PIPE_GREEN_HEAD_VERT		33111
#define ID_SPRITE_PIPE_GREEN_BODY_VERT		33131

#define ID_SPRITE_PIPE_BLACK_HEAD_VERT		33211
#define ID_SPRITE_PIPE_BLACK_BODY_VERT		33221

#define PIPE_DIRECTION_VERTICAL_UPSIDE		-1
#define PIPE_DIRECTION_VERTICAL_DOWNSIDE	1
#define PIPE_DIRECTION_HORIZONTAL			2

#define PIPE_APPEARANCE_TYPE_GREEN	1
#define PIPE_APPEARANCE_TYPE_BLACK	2

class CPipe : public CGameObject
{
protected:
	int height; // Unit: cell
	int	width; // Unit: cell
	float cellWidth;
	float cellHeight;

	int direction; // -1: vertical + upside, 1: vertical + downside, 2: horizontal
	int apperanceType; // 1: Green; 2: Black

	bool isMarioSpawnLocation; // Decides if mario can spawn from here
	bool isContainingPortal;
	bool isBlocking;

public:
	CPipe(float x, float y, int width, int height, float cellWidth, float cellHeight,
		int direction, int apperanceType, bool isMarioSpawnLocation, bool isContainingPortal) :CGameObject(x, y)
	{
		this->width = width;
		this->height = height;
		this->cellWidth = cellWidth;
		this->cellHeight = cellHeight;
		this->direction = direction;
		this->apperanceType = apperanceType;
		this->isMarioSpawnLocation = isMarioSpawnLocation;
		this->isContainingPortal = isContainingPortal;
		isBlocking = true;
	}
	virtual void Render();
	void GetSpriteId(int& spriteIdPipeHead, int& spriteIdPipeBody);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	bool IsMarioSpawnLocation() { return isMarioSpawnLocation; }
	bool IsContainingPortal() { return isContainingPortal; }

	int GetDirection() { return direction; }
	int IsBlocking() { return isBlocking; }
	void SetBlocking(bool isBlocking) { this->isBlocking = isBlocking; }
};
