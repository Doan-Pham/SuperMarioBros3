#include "Curtain.h"
#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"

void CCurtain::Render()
{
	if (this->height <= 0) return;
	float current_part_y = y;
	CSprites* sprites = CSprites::GetInstance();

	sprites->Get(ID_SPRITE_CURTAIN_END)->Draw(x, current_part_y);
	current_part_y -= CURTAIN_PART_HEIGHT;
	for (int i = 1; i < this->height; i++)
	{
		sprites->Get(ID_SPRITE_CURTAIN_BODY)->Draw(x, current_part_y);
		current_part_y -= CURTAIN_PART_HEIGHT;
	}
}

void CCurtain::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}