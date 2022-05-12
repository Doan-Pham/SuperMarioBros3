#include "KoopaRedNormal.h"
#include "Mario.h"
#include "PlatformGhost.h"

CKoopaRedNormal::CKoopaRedNormal(float x, float y, const LPPLAYSCENE& currentScene)
	:CGameObject(x, y), currentScene(currentScene)
{
	nx = -1;
	this->ay = KOOPA_NORMAL_GRAVITY;
	SetState(KOOPA_STATE_WALKING);
}

void CKoopaRedNormal::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_WALKING)
	{
		left = x - KOOPA_NORMAL_BBOX_WIDTH / 2;
		top = y - KOOPA_NORMAL_BBOX_HEIGHT / 2;
		right = left + KOOPA_NORMAL_BBOX_WIDTH;
		bottom = top + KOOPA_NORMAL_BBOX_HEIGHT;
	}
	else
	{
		left = x - KOOPA_SHELL_BBOX_WIDTH / 2;
		top = y - KOOPA_SHELL_BBOX_HEIGHT / 2;
		right = left + KOOPA_SHELL_BBOX_WIDTH;
		bottom = top + KOOPA_SHELL_BBOX_HEIGHT;
	}
}


void CKoopaRedNormal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	if (state == KOOPA_STATE_WALKING &&
		attachedBBox != NULL && 
		attachedBBox->GetState() == ATTACHED_BBOX_STATE_FALL)
	{
		vx = -vx;
		nx = -nx;
		attachedBBox->Delete();
		attachedBBox = new CAttachedBBox(x + nx * KOOPA_NORMAL_BBOX_WIDTH, y, vx, vy);
		this->currentScene->AddObject(attachedBBox);
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

	//DebugOut(L"[INFO] Koopa's Update() has been called \n");
}


void CKoopaRedNormal::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopaRedNormal::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CKoopaRedNormal*>(e->obj)) return;
	if (dynamic_cast<CMario*>(e->obj)) return;

	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;

	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}
	if (dynamic_cast<CPlatformGhost*>(e->obj))
		OnCollisionWithPlatformGhost(e);
}
void CKoopaRedNormal::OnCollisionWithPlatformGhost(LPCOLLISIONEVENT e)
{
	if (e->ny < 0)
	{

		CPlatformGhost* platform = dynamic_cast<CPlatformGhost*>(e->obj);
		float platform_l, platform_t, platform_r, platform_b;
		platform->GetBoundingBox(platform_l, platform_t, platform_r, platform_b);

		float koopa_l, koopa_t, koopa_r, koopa_b;
		this->GetBoundingBox(koopa_l, koopa_t, koopa_r, koopa_b);

		// Have to directly change koopa's y because ghost platform is non-blocking, so the 
		// collisions with it are not handled by the framework (inside that, the coordinates
		// of the src_obj that collided with blocking things are automatically adjusted
		// to be the correct numbers using some calculations with BLOCK_PUSH_FACTOR)
		y = platform_t - (koopa_b - koopa_t)/ 2 - BLOCK_PUSH_FACTOR_GHOST_PLATFORM;

		vy = 0;
	}
}

int CKoopaRedNormal::GetAniId()
{
	if (state == KOOPA_STATE_WALKING)
	{
		if (vx <= 0) return ID_ANI_KOOPA_WALKING_LEFT;
		else return ID_ANI_KOOPA_WALKING_RIGHT;
	}

	if (state == KOOPA_STATE_SHELL_DOWNSIDE_STILL)
		return ID_ANI_KOOPA_SHELL_DOWNSIDE_STILL;

	if (state == KOOPA_STATE_SHELL_UPSIDE_STILL)
		return ID_ANI_KOOPA_SHELL_UPSIDE_STILL;

	if (state == KOOPA_STATE_SHELL_DOWNSIDE_MOVING)
	{
		if (vx <= 0) return ID_ANI_KOOPA_SHELL_DOWNSIDE_MOVING_LEFT;
		else return ID_ANI_KOOPA_SHELL_DOWNSIDE_MOVING_RIGHT;
	}

	if (state == KOOPA_STATE_SHELL_UPSIDE_MOVING)
	{
		if (vx <= 0) return ID_ANI_KOOPA_SHELL_UPSIDE_MOVING_LEFT;
		else return ID_ANI_KOOPA_SHELL_UPSIDE_MOVING_RIGHT;
	}

}
void CKoopaRedNormal::Render()
{
	
	CAnimations::GetInstance()->Get(GetAniId())->Render(x, y);
	RenderBoundingBox();

	//DebugOut(L"[INFO] Koopa's Render() has been called \n");
}

void CKoopaRedNormal::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case KOOPA_STATE_WALKING:
		{
			vx = nx * KOOPA_NORMAL_WALKING_SPEED;
			
			if (attachedBBox == NULL)
			{
				attachedBBox = new CAttachedBBox(x + nx * KOOPA_NORMAL_BBOX_WIDTH, y, vx, vy);
				this->currentScene->AddObject(attachedBBox);
			}
			break;
		}
		case KOOPA_STATE_SHELL_DOWNSIDE_STILL:
		{
			if (attachedBBox != NULL)
			{
				attachedBBox->Delete();
				attachedBBox = NULL;
			}
			
			vx = 0;
			break;
		}

		case KOOPA_STATE_SHELL_DOWNSIDE_MOVING:
		{
			vx = nx * KOOPA_NORMAL_WALKING_SPEED;

			if (attachedBBox != NULL)
			{
				attachedBBox->Delete();
				attachedBBox = NULL;
			}
			break;
		}

		case KOOPA_STATE_SHELL_UPSIDE_STILL:
		{
			vx = 0;

			if (attachedBBox != NULL)
			{
				attachedBBox->Delete();
				attachedBBox = NULL;
			}
			break;
		}

		case KOOPA_STATE_SHELL_UPSIDE_MOVING:
		{
			vx = nx * KOOPA_NORMAL_WALKING_SPEED;

			if (attachedBBox != NULL)
			{
				attachedBBox->Delete();
				attachedBBox = NULL;
			}
			break;
		}

	}
}
