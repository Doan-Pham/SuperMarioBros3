#pragma once

#include <windows.h>
#include "debug.h"

#define P_METER_MIN_VALUE				0
#define P_METER_MAX_VALUE				120

// This rate means every "x" miliseconds, the p-meter increases by 1 increment
#define P_METER_INCREASING_RATE			250

// This rate means every "x" miliseconds, the p-meter decreases by 1 increment
#define P_METER_DECREASING_RATE			400

#define P_METER_INCREASING_INCREMENT	20
#define P_METER_DECREASING_INCREMENT	20

#define P_METER_STATE_INCREASING		100
#define P_METER_STATE_DECREASING		200

class CPMeter
{
	int state;
	int currentValue;
	int minValue;
	int maxValue;

	// Unit: number of times P-meter increases/milliseconds
	int increasingRate;

	// Unit: number of times P-meter decreases/milliseconds
	int decreasingRate;

	int increasingIncrement;
	int decreasingIncrement;

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

		increase_start = -1;
		decrease_start = -1;
	}
	void SetState(int state);
	bool isFullyCharged() { return currentValue == maxValue; }
};

typedef CPMeter* LPPMETER;