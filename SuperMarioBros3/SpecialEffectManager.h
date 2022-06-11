#pragma once

#include "SpecialEffect.h"
#include "PlayScene.h"
#include "Game.h"
#include "debug.h"

#define EFFECT_TYPE_COIN_APPEAR				1
#define EFFECT_TYPE_SCORES_APPEAR			2

#define EFFECT_TYPE_TAIL_ATTACK				3
#define EFFECT_TYPE_FIREBALL_COLLIDE		4
#define EFFECT_TYPE_PLANT_DIE				5
#define EFFECT_TYPE_BRICK_GLASS_BROKEN		6

#define EFFECT_DEFAULT_SPEED_X				0.0f
#define EFFECT_DEFAULT_SPEED_Y				0.0f
#define	EFFECT_DEFAULT_TIME					100

#define EFFECT_COIN_APPEAR_SPEED_Y			0.2f

#define ID_ANI_EFFECT_TAIL_ATTACK				8510
#define ID_ANI_EFFECT_FIREBALL_COLLIDE			8520
#define ID_ANI_EFFECT_PLANT_DIE					8521

class CSpecialEffectManager
{
public:
	static void CreateSpecialEffect(float x, float y, int effectType, int scores = 0)
	{
		int animationId = -1;
		int animationTime = EFFECT_DEFAULT_TIME;

		float vx = EFFECT_DEFAULT_SPEED_X;
		float vy = EFFECT_DEFAULT_SPEED_Y;
		switch (effectType)
		{
		case EFFECT_TYPE_COIN_APPEAR:
		{
			vy = EFFECT_COIN_APPEAR_SPEED_Y;
			break;
		}

		case EFFECT_TYPE_SCORES_APPEAR:
		{
			break;
		}

		case EFFECT_TYPE_TAIL_ATTACK:
		{
			animationId = ID_ANI_EFFECT_TAIL_ATTACK;
			break;
		}

		case EFFECT_TYPE_FIREBALL_COLLIDE:
		{
			animationId = ID_ANI_EFFECT_FIREBALL_COLLIDE;
			break;
		}

		case EFFECT_TYPE_PLANT_DIE:
		{
			animationId = ID_ANI_EFFECT_PLANT_DIE;
			break;
		}


		default:
			DebugOut(L"[ERROR] Can't find special effect type: %i", effectType);
			break;
		}
		CSpecialEffect* effect = new CSpecialEffect(x, y, vx, vy, animationId, animationTime);
		CPlayScene* currentScene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
		currentScene->AddObject(effect);
	}
};

