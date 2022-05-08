#pragma once

#include <windows.h>
#include "debug.h"

#define P_METER_MIN_VALUE				0
#define P_METER_MAX_VALUE				100

// This rate means every "x" miliseconds, the p-meter increases by 1 increment
#define P_METER_INCREASING_RATE			500

// This rate means every "x" miliseconds, the p-meter decreases by 1 increment
#define P_METER_DECREASING_RATE			800


#define P_METER_INCREASING_INCREMENT	20
#define P_METER_DECREASING_INCREMENT	10

#define P_METER_STATE_INCREASING		100
#define P_METER_STATE_DECREASING		200

class CPMeter
{
	int state;
	int currentValue;
	int minValue;
	int maxValue;

	// Unit: number of increases/milliseconds
	int increasingRate;

	// Unit: number of decreases/milliseconds
	int decreasingRate;

	ULONGLONG increase_start;
	ULONGLONG decrease_start;

public:
	CPMeter()
	{
		minValue = P_METER_MIN_VALUE;
		maxValue = P_METER_MAX_VALUE;
		increasingRate = P_METER_INCREASING_RATE;
		decreasingRate = P_METER_DECREASING_RATE;
		currentValue = minValue;

		increase_start = -1;
		decrease_start = -1;
	}
	void SetState(int state);
};

typedef CPMeter* LPPMETER;