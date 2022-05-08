#include "PMeter.h"

void CPMeter::SetState(int state)
{
	DWORD tickPerIncrease = 1000 / increasingRate;
	DWORD tickPerDecrease = 1000 / decreasingRate;

	switch (state)
	{
	case P_METER_STATE_INCREASING:
		if ((DWORD)(GetTickCount64() - increase_start) >= tickPerIncrease && 
			(currentValue + increasingRate) <= maxValue)
		{
			currentValue += P_METER_INCREASING_RATE;
			increase_start = GetTickCount64();
		}
		break;

	case P_METER_STATE_DECREASING:
		if ((DWORD)(GetTickCount64() - decrease_start) >= tickPerIncrease && currentValue > minValue)
		{
			currentValue--;
			decrease_start = GetTickCount64();
		}
			
		break;
	}

	DebugOutTitle(L"minValue = %d, maxValue = %d, currentValue = %d,", minValue, maxValue, currentValue);
}
