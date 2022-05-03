#pragma once

#include "Item.h"
#include "BrickQuestionMark.h"

#define ID_SPRITE_LEAF_MOVING_LEFT 41311
#define ID_SPRITE_LEAF_MOVING_RIGHT 41321

#define LEAF_GRAVITY 0.0001f
#define LEAF_MOVING_SPEED 0.06f
#define LEAF_APPEARING_SPEED 0.1f

#define LEAF_BOUNCE_DISTANCE 35.0f
#define LEAF_MAX_X_DISTANCE 40.0f

#define LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 16

#define LEAF_STATE_HIDING 100
#define LEAF_STATE_APPEARING 200
#define LEAF_STATE_MOVING 300

#define LEAF_SCORES_GIVEN_WHEN_HIT 1000

class CLeaf : public CItem
{
protected:
	float ax;
	float ay;
	float y_destination;
	float x_start;
	float x_end;
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual int IsBlocking() { return 0; }

	virtual bool IsHidden() { return (state == LEAF_STATE_HIDING); }

public:
	CLeaf(float x, float y);
	virtual void SetState(int state);
	virtual int GetScoresGivenWhenHit() { return LEAF_SCORES_GIVEN_WHEN_HIT; }
};




