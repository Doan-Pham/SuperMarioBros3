#include "Pipe.h"

void CPipe::Render()
{
	switch (direction)
	{
	if (this->width <= 0 || this->height <= 0) return;

	case PIPE_DIRECTION_VERTICAL_UPSIDE:
	{
		float current_part_y = y;
		CSprites* sprites = CSprites::GetInstance();

		sprites->Get(ID_SPRITE_PIPE_HEAD_VERT_UPSIDE)->Draw(x, current_part_y);
		current_part_y += this->cellHeight;
		for (int i = 1; i < this->height; i++)
		{
			sprites->Get(ID_SPRITE_PIPE_BODY_VERT)->Draw(x, current_part_y);
			current_part_y += this->cellHeight;
		}
		break;
	}

	case PIPE_DIRECTION_VERTICAL_DOWNSIDE:
	{
		break;
	}

	case PIPE_DIRECTION_HORIZONTAL:
	{
		break;
	}
	}
}

void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - (width /2)*cellWidth ;
	t = y - this->cellHeight / 2;
	r = l + this->cellWidth * this->width;
	b = t + this->cellHeight * this->height;

}
