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

#define EFFECT_DEFAULT_SPEED				0.0f
#define EFFECT_DEFAULT_ACCEL				0.0f
#define	EFFECT_DEFAULT_TIME					100

#define EFFECT_COIN_APPEAR_SPEED_Y			0.35f
#define EFFECT_COIN_APPEAR_GRAVITY			0.001f
#define EFFECT_COIN_APPEAR_TIME				600

#define ID_ANI_EFFECT_COIN_APPEAR				8530
#define ID_ANI_EFFECT_TAIL_ATTACK				8510
#define ID_ANI_EFFECT_FIREBALL_COLLIDE			8520
#define ID_ANI_EFFECT_PLANT_DIE					8521

class CSpecialEffectManager
{
public:
	static void CreateSpecialEffect(float x, float y, int effectType, int scores = 0)
	{
		float ax = EFFECT_DEFAULT_ACCEL;
		float ay = EFFECT_DEFAULT_ACCEL;
		float vx = EFFECT_DEFAULT_SPEED;
		float vy = EFFECT_DEFAULT_SPEED;

		int animationId = -1;
		int animationTime = EFFECT_DEFAULT_TIME;

		switch (effectType)
		{
		case EFFECT_TYPE_COIN_APPEAR:
		{
			ay = EFFECT_COIN_APPEAR_GRAVITY;
			vy = -EFFECT_COIN_APPEAR_SPEED_Y;
			animationId = ID_ANI_EFFECT_COIN_APPEAR;
			animationTime = EFFECT_COIN_APPEAR_TIME;
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
		CSpecialEffect* effect = new CSpecialEffect(x, y, vx, vy, ax, ay, animationId, animationTime);
		CPlayScene* currentScene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
		currentScene->AddObject(effect);
	}
};

