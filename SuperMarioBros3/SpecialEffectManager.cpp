#include "SpecialEffectManager.h"

void CSpecialEffectManager::CreateSpecialEffect(float x, float y, int effectType, int scores)
{
	if (!dynamic_cast<LPPLAYSCENE>(CGame::GetInstance()->GetCurrentScene())) return;
	float ax = EFFECT_DEFAULT_ACCEL;
	float ay = EFFECT_DEFAULT_ACCEL;
	float vx = EFFECT_DEFAULT_SPEED;
	float vy = EFFECT_DEFAULT_SPEED;

	int animationId = -1;
	int animationTime = EFFECT_DEFAULT_TIME;
	CSpecialEffect* effect;

	switch (effectType)
	{
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
		ay = EFFECT_SCORES_APPEAR_GRAVITY;
		vy = -EFFECT_SCORES_APPEAR_SPEED_Y;
		animationTime = EFFECT_SCORES_APPEAR_TIME;
		switch (scores)
		{
		case SCORES_LVL1:
		{
			animationId = ID_ANI_EFFECT_SCORES_APPEAR_LVL1;
			break;
		}

		case SCORES_LVL2:
		{
			animationId = ID_ANI_EFFECT_SCORES_APPEAR_LVL2;
			break;
		}

		case SCORES_LVL3:
		{
			animationId = ID_ANI_EFFECT_SCORES_APPEAR_LVL3;
			break;
		}

		case SCORES_LVL4:
		{
			animationId = ID_ANI_EFFECT_SCORES_APPEAR_LVL4;
			break;
		}

		case SCORES_LVL5:
		{
			animationId = ID_ANI_EFFECT_SCORES_APPEAR_LVL5;
			break;
		}

		case SCORES_LVL6:
		{
			animationId = ID_ANI_EFFECT_SCORES_APPEAR_LVL6;
			break;
		}

		case SCORES_LVL7:
		{
			animationId = ID_ANI_EFFECT_SCORES_APPEAR_LVL7;
			break;
		}

		case SCORES_LVL8:
		{
			animationId = ID_ANI_EFFECT_SCORES_APPEAR_LVL8;
			break;
		}
		default:
		{
			DebugOut(L"The amount of acquired scores is invalid: %i ", scores);
			return;
		}
		}
		break;
	}

	case EFFECT_TYPE_1UP_APPEAR:
	{
		ay = EFFECT_SCORES_APPEAR_GRAVITY;
		vy = -EFFECT_SCORES_APPEAR_SPEED_Y;
		animationTime = EFFECT_SCORES_APPEAR_TIME;
		animationId = ID_ANI_EFFECT_SCORES_APPEAR_1UP;
		break;
	}

	case EFFECT_TYPE_BRICK_GLASS_BROKEN:
	{
		CPlayScene* currentScene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
		effect = new CSpecialEffect(
			x, y,
			EFFECT_BRICK_GLASS_BROKEN_APPEAR_SPEED_X,
			-EFFECT_BRICK_GLASS_BROKEN_APPEAR_SPEED_Y_HIGH,
			ax, EFFECT_BRICK_GLASS_BROKEN_APPEAR_GRAVITY,
			ID_ANI_EFFECT_BRICK_GLASS_BROKEN,
			EFFECT_BRICK_GLASS_BROKEN_APPEAR_TIME);

		currentScene->AddObject(effect);

		effect = new CSpecialEffect(
			x, y,
			EFFECT_BRICK_GLASS_BROKEN_APPEAR_SPEED_X,
			-EFFECT_BRICK_GLASS_BROKEN_APPEAR_SPEED_Y_LOW,
			ax, EFFECT_BRICK_GLASS_BROKEN_APPEAR_GRAVITY,
			ID_ANI_EFFECT_BRICK_GLASS_BROKEN,
			EFFECT_BRICK_GLASS_BROKEN_APPEAR_TIME);

		currentScene->AddObject(effect);

		effect = new CSpecialEffect(
			x, y,
			-EFFECT_BRICK_GLASS_BROKEN_APPEAR_SPEED_X,
			-EFFECT_BRICK_GLASS_BROKEN_APPEAR_SPEED_Y_HIGH,
			ax, EFFECT_BRICK_GLASS_BROKEN_APPEAR_GRAVITY,
			ID_ANI_EFFECT_BRICK_GLASS_BROKEN,
			EFFECT_BRICK_GLASS_BROKEN_APPEAR_TIME);

		currentScene->AddObject(effect);

		effect = new CSpecialEffect(
			x, y,
			-EFFECT_BRICK_GLASS_BROKEN_APPEAR_SPEED_X,
			-EFFECT_BRICK_GLASS_BROKEN_APPEAR_SPEED_Y_LOW,
			ax, EFFECT_BRICK_GLASS_BROKEN_APPEAR_GRAVITY,
			ID_ANI_EFFECT_BRICK_GLASS_BROKEN,
			EFFECT_BRICK_GLASS_BROKEN_APPEAR_TIME);

		currentScene->AddObject(effect);
		return;
		break;
	}
	default:
		DebugOut(L"[ERROR] Can't find special effect type: %i", effectType);
		break;
	}
	effect = new CSpecialEffect(x, y, vx, vy, ax, ay, animationId, animationTime);

	// In case some effects are added in intro scene

	CPlayScene* currentScene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	currentScene->AddObject(effect);
}
