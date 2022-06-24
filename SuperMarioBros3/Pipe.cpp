#include "Pipe.h"

void CPipe::Render()
{
	if (this->width <= 0 || this->height <= 0) return;

	int spriteIdPipeHead, spriteIdPipeBody;
	GetSpriteId(spriteIdPipeHead, spriteIdPipeBody);

	switch (direction)
	{
	//RenderBoundingBox();
	case PIPE_DIRECTION_VERTICAL_UPSIDE:
	{
		float current_part_y = y;
		CSprites* sprites = CSprites::GetInstance();

		sprites->Get(spriteIdPipeHead)->Draw(x, current_part_y);
		current_part_y += this->cellHeight;
		for (int i = 1; i < this->height; i++)
		{
			sprites->Get(spriteIdPipeBody)->Draw(x, current_part_y);
			current_part_y += this->cellHeight;
		}
		break;
	}

	case PIPE_DIRECTION_VERTICAL_DOWNSIDE:
	{
		float current_part_y = y;
		CSprites* sprites = CSprites::GetInstance();

		sprites->Get(spriteIdPipeHead)->Draw(x, current_part_y);
		current_part_y -= this->cellHeight;
		for (int i = 1; i < this->height; i++)
		{
			sprites->Get(spriteIdPipeBody)->Draw(x, current_part_y);
			current_part_y -= this->cellHeight;
		}
		break;
	}

	case PIPE_DIRECTION_HORIZONTAL:
	{
		break;
	}
	
	}

	//DebugOutTitle(L"isBlocking %i ", isBlocking);
}

void CPipe::GetSpriteId(int& spriteIdPipeHead, int& spriteIdPipeBody)
{

	switch (apperanceType)
	{
	case PIPE_APPEARANCE_TYPE_GREEN:
	{
		if (direction == PIPE_DIRECTION_VERTICAL_DOWNSIDE || direction == PIPE_DIRECTION_VERTICAL_UPSIDE)
		{
			spriteIdPipeHead = ID_SPRITE_PIPE_GREEN_HEAD_VERT;
			spriteIdPipeBody = ID_SPRITE_PIPE_GREEN_BODY_VERT;
		}
		break;

	}
	case PIPE_APPEARANCE_TYPE_BLACK:
	{
		if (direction == PIPE_DIRECTION_VERTICAL_DOWNSIDE || direction == PIPE_DIRECTION_VERTICAL_UPSIDE)
		{
			spriteIdPipeHead = ID_SPRITE_PIPE_BLACK_HEAD_VERT;
			spriteIdPipeBody = ID_SPRITE_PIPE_BLACK_BODY_VERT;
		}
		break;
	}
	default:
	{
		DebugOut(L"[ERROR] Pipe doesn't have appearance type : %i", apperanceType);
		return;
	}
	}
}
void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - (width *cellWidth)/2 ;
	if (direction == PIPE_DIRECTION_VERTICAL_UPSIDE)
		t = y - this->cellHeight / 2;
	else if (direction == PIPE_DIRECTION_VERTICAL_DOWNSIDE)
		t = y + cellHeight / 2 - height * cellHeight;

	r = l + this->cellWidth * this->width;
	b = t + this->cellHeight * this->height;

}
