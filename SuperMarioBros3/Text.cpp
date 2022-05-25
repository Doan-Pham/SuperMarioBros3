#include "Text.h"
#include "debug.h"
#include "Sprites.h"

void CText::Render()
{
	if (characters.size() < 1) return;
	int spriteId = -1;
	float single_char_x = x;
	for (char character : characters)
	{
		spriteId = getSpriteId(character);
		if (spriteId == -1)
		{
			DebugOut(L"[ERROR] Can't find sprite id for character %c - ascii code : %i", 
				character, character);
			return;
		}

		// Skip if character is blank space
		else if (spriteId != 999)
			CSprites::GetInstance()->Get(spriteId)->Draw(single_char_x, y);
		
		single_char_x += CHARACTER_STANDARD_WIDTH;
	}
}

int CText::getSpriteId(char character) 
{
	int spriteId = -1;
	if (character == ASCII_CODE_CHARACTER_SPACE)
	{
		spriteId = 999;
	}
	if (character >= ASCII_CODE_CHARACTER_0 && character <= ASCII_CODE_CHARACTER_9)
	{
		spriteId = ID_SPRITE_CHARACTER_0 + (character - ASCII_CODE_CHARACTER_0);
	}
	else if (character >= ASCII_CODE_CHARACTER_A && character <= ASCII_CODE_CHARACTER_Z)
	{
		spriteId = ID_SPRITE_CHARACTER_A + (character - ASCII_CODE_CHARACTER_A);
	}
	else if (character >= ASCII_CODE_CHARACTER_a && character <= ASCII_CODE_CHARACTER_z)
	{
		spriteId = ID_SPRITE_CHARACTER_A + (character - ASCII_CODE_CHARACTER_a);
	}
	return spriteId;
}