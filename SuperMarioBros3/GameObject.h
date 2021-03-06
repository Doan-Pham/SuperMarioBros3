#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>

#include "Animation.h"
#include "Animations.h"
#include "Sprites.h"
#include "Collision.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define BBOX_ALPHA 0.25f		// Bounding box transparency

class CGameObject
{
protected:
	int id;
	float x; 
	float y;

	float vx;
	float vy;

	int nx;	 

	int state;

	bool isDeleted; 
	bool isHidden;

	int current_first_grid_row_index, current_first_grid_col_index, 
		current_last_grid_row_index, current_last_grid_col_index;

public: 
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	int GetId() { return this->id; }
	void SetId(int id) { this->id = id; }

	void GetCurrentGrid(
		int& current_first_grid_row_index, int& current_first_grid_col_index, 
		int& current_last_grid_row_index, int& current_last_grid_col_index)
	{
		current_first_grid_row_index = this->current_first_grid_row_index;
		current_first_grid_col_index = this->current_first_grid_col_index;
		current_last_grid_row_index = this->current_last_grid_row_index;
		current_last_grid_col_index = this->current_last_grid_col_index;
	}
	void SetCurrentGrid(
		int current_first_grid_row_index, int current_first_grid_col_index, 
		int current_last_grid_row_index, int current_last_grid_col_index)
	{
		this->current_first_grid_row_index = current_first_grid_row_index;
		this->current_first_grid_col_index = current_first_grid_col_index;
		this->current_last_grid_row_index = current_last_grid_row_index;
		this->current_last_grid_col_index = current_last_grid_col_index;
	}

	int GetState() { return this->state; }

	virtual void Delete() { isDeleted = true;  }
	bool IsDeleted() { return isDeleted; }

	void RenderBoundingBox();

	CGameObject();
	CGameObject(float x, float y) :CGameObject() { this->x = x; this->y = y; }


	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }

	//
	// Collision ON or OFF ? This can change depending on object's state. For example: die
	//
	virtual int IsCollidable() { return 0; };

	// When no collision has been detected (triggered by CCollision::Process)
	virtual void OnNoCollision(DWORD dt) {};

	// When collision with an object has been detected (triggered by CCollision::Process)
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {};
	
	// Is this object blocking other object? If YES, collision framework will automatically push the other object
	virtual int IsBlocking() { return 1; }

	// If this method return 1, the collision framework will ignore the object's collisions with blocks
	virtual int IsSlippingThroughBlocks() { return 0; }


	//If an object is hidden, player has to hit the container for that hidden object first to
	//make that object appear, only then mario can collide with that hidden object
	virtual bool IsHidden() { return isHidden; }
	virtual void Hide() { isHidden = true; }
	virtual void UnHide() { isHidden = false; }

	//The amount of scores, coins, and lives an object gives when hit/destroyed
	virtual int GetScoresGivenWhenHit() { return 0; }
	virtual int GetCoinsGivenWhenHit() { return 0; }
	virtual int GetLivesGivenWhenHit() { return 0; }

	// Objects with higher priority are rendered first and can be covered by others
	virtual int GetRenderPriority() { return 0; }

	~CGameObject();

	static bool IsDeleted(const LPGAMEOBJECT &o) { return o->isDeleted; }
};

