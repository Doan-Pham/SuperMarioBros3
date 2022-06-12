#pragma once

#include "SpecialEffect.h"
#include "PlayScene.h"
#include "Game.h"
#include "debug.h"


#define EFFECT_TYPE_TAIL_ATTACK				1
#define EFFECT_TYPE_FIREBALL_COLLIDE		2
#define EFFECT_TYPE_PLANT_DIE				3

#define EFFECT_TYPE_COIN_APPEAR				4
#define EFFECT_TYPE_SCORES_APPEAR			5
#define EFFECT_TYPE_1UP_APPEAR				6
#define EFFECT_TYPE_BRICK_GLASS_BROKEN		7

#define EFFECT_DEFAULT_SPEED				0.0f
#define EFFECT_DEFAULT_ACCEL				0.0f
#define	EFFECT_DEFAULT_TIME					200

#define EFFECT_COIN_APPEAR_SPEED_Y			0.35f
#define EFFECT_COIN_APPEAR_GRAVITY			0.001f
#define EFFECT_COIN_APPEAR_TIME				600

#define EFFECT_SCORES_APPEAR_SPEED_Y		0.1f
#define EFFECT_SCORES_APPEAR_GRAVITY		0.00015f
#define EFFECT_SCORES_APPEAR_TIME			800

#define EFFECT_BRICK_GLASS_BROKEN_APPEAR_SPEED_Y_HIGH		0.25f
#define EFFECT_BRICK_GLASS_BROKEN_APPEAR_SPEED_Y_LOW		0.15f
#define EFFECT_BRICK_GLASS_BROKEN_APPEAR_SPEED_X			0.05f
#define EFFECT_BRICK_GLASS_BROKEN_APPEAR_GRAVITY			0.0005f
#define EFFECT_BRICK_GLASS_BROKEN_APPEAR_TIME				3000

#define ID_ANI_EFFECT_TAIL_ATTACK				8510
#define ID_ANI_EFFECT_FIREBALL_COLLIDE			8520
#define ID_ANI_EFFECT_PLANT_DIE					8521
#define ID_ANI_EFFECT_COIN_APPEAR				8530

#define ID_ANI_EFFECT_SCORES_APPEAR_LVL1		8541
#define ID_ANI_EFFECT_SCORES_APPEAR_LVL2		8542
#define ID_ANI_EFFECT_SCORES_APPEAR_LVL3		8543
#define ID_ANI_EFFECT_SCORES_APPEAR_LVL4		8544
#define ID_ANI_EFFECT_SCORES_APPEAR_LVL5		8545
#define ID_ANI_EFFECT_SCORES_APPEAR_LVL6		8546
#define ID_ANI_EFFECT_SCORES_APPEAR_LVL7		8547
#define ID_ANI_EFFECT_SCORES_APPEAR_LVL8		8548
#define ID_ANI_EFFECT_SCORES_APPEAR_1UP			8549

#define ID_ANI_EFFECT_BRICK_GLASS_BROKEN		8550

class CSpecialEffectManager
{
public:
	static void CreateSpecialEffect(float x, float y, int effectType, int scores = 0);
};

