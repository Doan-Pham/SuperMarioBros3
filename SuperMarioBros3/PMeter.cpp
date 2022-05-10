#include "PMeter.h"

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

		if ((DWORD)(GetTickCount64() - increase_start) >= increasingRate)
		{
			if (currentValue + increasingIncrement > maxValue) currentValue = maxValue;
			else currentValue += increasingIncrement;

			increase_start = GetTickCount64();
		}
		break;

	case P_METER_STATE_DECREASING:
		if ((DWORD)(GetTickCount64() - decrease_start) >= decreasingRate)
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
