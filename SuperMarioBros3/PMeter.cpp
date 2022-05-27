#include "PMeter.h"

void CPMeter::Render()
{
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();

	float current_charge_level_x = x;
	float current_charge_level_value = P_METER_CHARGE_LEVEL_VALUE;
	while (current_charge_level_value <= P_METER_MAX_VALUE)
	{
		if (currentValue >= current_charge_level_value)
			sprites->Get(ID_SPRITE_P_METER_ARROW_CHARGED)->Draw(current_charge_level_x, y);
		else 
			sprites->Get(ID_SPRITE_P_METER_ARROW_NOT_CHARGED)->Draw(current_charge_level_x, y);

		if (current_charge_level_value == P_METER_MAX_VALUE)
			current_charge_level_x += P_METER_P_BUTTON_WIDTH;
		else
			current_charge_level_x += P_METER_ARROW_WIDTH;

		current_charge_level_value += P_METER_CHARGE_LEVEL_VALUE;
	}
	if (currentValue == maxValue)
		animations->Get(ID_ANI_P_METER_P_BUTTON_FULL_CHARGED)->Render(current_charge_level_x, y);
	else 
		sprites->Get(ID_SPRITE_P_METER_P_BUTTON_NOT_CHARGED)->Draw(current_charge_level_x, y);
}

void CPMeter::SetState(int state)
{

	switch (state)
	{
	//case P_METER_STATE_KEY_A_HIT:
	//	
	//	key_A_hit_start = GetTickCount64();
	//	break;

	case P_METER_STATE_INCREASING:
		//if ((DWORD)(GetTickCount64() - key_A_hit_start) >= P_METER_WAIT_BEFORE_INCREASE)
		//	increase_start = GetTickCount64();

		if ((GetTickCount64() - increase_start) >= increasingRate)
		{
			if (currentValue + increasingIncrement > maxValue) currentValue = maxValue;
			else currentValue += increasingIncrement;

			increase_start = GetTickCount64();
		}
		break;

	case P_METER_STATE_DECREASING:
		if ((GetTickCount64() - decrease_start) >= decreasingRate)
		{
			if (currentValue - decreasingIncrement < minValue) currentValue = minValue;
			else currentValue -= decreasingIncrement;

			decrease_start = GetTickCount64();
		}
		//key_A_hit_start = GetTickCount64();
		break;
	}

	//DebugOutTitle(L"minValue = %d, maxValue = %d, currentValue = %d,", minValue, maxValue, currentValue);
}
