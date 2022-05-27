#pragma once

#include <windows.h>
#include "debug.h"
#include "GameObject.h"


#define ID_SPRITE_P_METER_ARROW_NOT_CHARGED		83211
#define ID_SPRITE_P_METER_P_BUTTON_NOT_CHARGED	83212

#define ID_SPRITE_P_METER_ARROW_CHARGED			83221
#define ID_SPRITE_P_METER_P_BUTTON_CHARGED		83222

#define ID_ANI_P_METER_P_BUTTON_FULL_CHARGED	8321

#define P_METER_ARROW_WIDTH		8
#define P_METER_P_BUTTON_WIDTH	15

// The value p-meter needs to reach to fill one charge level
#define P_METER_CHARGE_LEVEL_VALUE		20
#define P_METER_MIN_VALUE				0
#define P_METER_MAX_VALUE				120

// In the original game, after the player click "A", the game waits for a second before increasing 
// the p-meter
#define P_METER_WAIT_BEFORE_INCREASE	1000

// This rate means every "x" miliseconds, the p-meter increases by 1 increment
#define P_METER_INCREASING_RATE			250

// This rate means every "x" miliseconds, the p-meter decreases by 1 increment
#define P_METER_DECREASING_RATE			400

#define P_METER_INCREASING_INCREMENT	20
#define P_METER_DECREASING_INCREMENT	20

#define P_METER_STATE_KEY_A_HIT			100
#define P_METER_STATE_INCREASING		200
#define P_METER_STATE_DECREASING		300

class CPMeter : public CGameObject
{
	int state;
	int currentValue;
	int minValue;
	int maxValue;

	// Unit: milliseconds per 1 timer P-meter increases
	int increasingRate;

	// Unit: milliseconds per 1 timer P-meter decreases
	int decreasingRate;

	int increasingIncrement;
	int decreasingIncrement;

	// In the original game, after the player click "A", the game waits for a second before increasing 
	// the p-meter
	//ULONGLONG key_A_hit_start;

	ULONGLONG increase_start;
	ULONGLONG decrease_start;

public:
	CPMeter()
	{
		minValue = P_METER_MIN_VALUE;
		maxValue = P_METER_MAX_VALUE;
		increasingRate = P_METER_INCREASING_RATE;
		decreasingRate = P_METER_DECREASING_RATE;

		increasingIncrement = P_METER_INCREASING_INCREMENT;
		decreasingIncrement = P_METER_DECREASING_INCREMENT;
		currentValue = minValue;

		state = P_METER_STATE_DECREASING;

		increase_start = -1;
		decrease_start = -1;

		//key_A_hit_start = -1;
		//increase_start = 9999999999;
	}
	void Render();
	void SetState(int state);
	bool IsFullyCharged() { return currentValue == maxValue; }
	int GetChargeLevel() { return currentValue / P_METER_CHARGE_LEVEL_VALUE; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
};

typedef CPMeter* LPPMETER;