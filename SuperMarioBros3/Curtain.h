#include "GameObject.h"

#define ID_SPRITE_CURTAIN_BODY		84101
#define ID_SPRITE_CURTAIN_END		84102

#define CURTAIN_PART_HEIGHT			16
#define CURTAIN_SPEED_Y				0.1f

class CCurtain : public CGameObject
{
protected:
	int height;				// Unit: cell 

public:
	CCurtain(float x, float y, float height) :CGameObject(x, y)
	{
		this->height = height;
		this->vy = -CURTAIN_SPEED_Y;
	}

	void Render(); 
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) { y += vy * dt; };
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

typedef CCurtain* LPCURTAIN;