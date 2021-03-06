#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id = -1;	// target scene to switch to 
	int map_id = -1;	// target map to switch to

	float width;
	float height; 
public:
	CPortal(float x, float y, float width, float height, int scene_id, int map_id);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	void RenderBoundingBox(void);
	
	int GetSceneId() { return scene_id;  }
	int GetMapId() { return map_id; }
	int IsBlocking() { return 0; }
};