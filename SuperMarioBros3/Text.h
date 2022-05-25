#pragma once

#include "GameObject.h"

#define CHARACTER_STANDARD_WIDTH	8
#define CHARACTER_STANDARD_HEIGHT	8

#define ASCII_CODE_CHARACTER_SPACE	32

#define ASCII_CODE_CHARACTER_0		48
#define ASCII_CODE_CHARACTER_9		57

#define ASCII_CODE_CHARACTER_A		65
#define ASCII_CODE_CHARACTER_Z		90

#define ASCII_CODE_CHARACTER_a		97
#define ASCII_CODE_CHARACTER_z		122

#define ID_SPRITE_CHARACTER_0		82001
#define ID_SPRITE_CHARACTER_9		82010

#define ID_SPRITE_CHARACTER_A		82011
#define ID_SPRITE_CHARACTER_Z		82036


// A class to draw text onto screen
class CText : public CGameObject
{
protected:
	vector<char> characters;
public:
	CText(float x, float y, vector<char> characters) : CGameObject(x, y)
	{
		this->characters = characters;
	}
	int IsBlocking() { return 0; }
	int getSpriteId(char character);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
};

