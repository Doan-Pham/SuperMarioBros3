#pragma once

//
// This file contains all IDs of game assets 
//

#define ID_TEX_MARIO 1
#define ID_TEX_BLOCK 3
#define ID_TEX_ITEM 4
#define ID_TEX_ENEMY 5
#define ID_TEX_TILESET_1_1 11

//#define ID_TEX_MISC 20

#define ID_MAP_1_1			11


#define OBJECT_TYPE_MARIO	10

#define OBJECT_TYPE_BLOCK	20
#define OBJECT_TYPE_BLOCK_BRICK_QUESTIONMARK	(OBJECT_TYPE_BLOCK + 1)

#define OBJECT_TYPE_ITEM	30
#define OBJECT_TYPE_ITEM_MUSHROOM_BIG	(OBJECT_TYPE_ITEM + 1)
#define OBJECT_TYPE_ITEM_MUSHROOM_UP	(OBJECT_TYPE_ITEM + 2)
#define OBJECT_TYPE_ITEM_LEAF	(OBJECT_TYPE_ITEM + 3)
#define OBJECT_TYPE_ITEM_COIN	(OBJECT_TYPE_ITEM + 15)


#define OBJECT_TYPE_PLATFORM 50
#define OBJECT_TYPE_PLATFORM_TILE (OBJECT_TYPE_PLATFORM + 1)
#define OBJECT_TYPE_PLATFORM_ONE_LAYER (OBJECT_TYPE_PLATFORM + 2)
#define OBJECT_TYPE_PLATFORM_GHOST (OBJECT_TYPE_PLATFORM + 3)

#define OBJECT_TYPE_ENEMY	60
#define OBJECT_TYPE_ENEMY_GOOMBA	(OBJECT_TYPE_ENEMY + 1)


#define OBJECT_TYPE_PORTAL	0


#pragma region MARIO 


#define ID_SPRITE_MARIO 10000

#pragma region MARIO_BIG
#define ID_SPRITE_MARIO_BIG (ID_SPRITE_MARIO + 1000)

#define ID_SPRITE_MARIO_BIG_IDLE (ID_SPRITE_MARIO_BIG + 100)
#define ID_SPRITE_MARIO_BIG_IDLE_LEFT (ID_SPRITE_MARIO_BIG_IDLE + 10)
#define ID_SPRITE_MARIO_BIG_IDLE_RIGHT (ID_SPRITE_MARIO_BIG_IDLE + 20)

#define ID_SPRITE_MARIO_BIG_WALKING (ID_SPRITE_MARIO_BIG + 200)
#define ID_SPRITE_MARIO_BIG_WALKING_LEFT (ID_SPRITE_MARIO_BIG_WALKING + 10)
#define ID_SPRITE_MARIO_BIG_WALKING_RIGHT (ID_SPRITE_MARIO_BIG_WALKING + 20)

#define ID_SPRITE_MARIO_BIG_RUNNING (ID_SPRITE_MARIO_BIG + 300)
#define ID_SPRITE_MARIO_BIG_RUNNING_LEFT (ID_SPRITE_MARIO_BIG_RUNNING + 10)
#define ID_SPRITE_MARIO_BIG_RUNNING_RIGHT (ID_SPRITE_MARIO_BIG_RUNNING + 20)


#define ID_SPRITE_MARIO_BIG_JUMP_WALK (ID_SPRITE_MARIO_BIG + 400)
#define ID_SPRITE_MARIO_BIG_JUMP_WALK_LEFT (ID_SPRITE_MARIO_BIG_JUMP_WALK + 10)
#define ID_SPRITE_MARIO_BIG_JUMP_WALK_RIGHT (ID_SPRITE_MARIO_BIG_JUMP_WALK + 20)

#define ID_SPRITE_MARIO_BIG_SIT (ID_SPRITE_MARIO_BIG + 500)
#define ID_SPRITE_MARIO_BIG_SIT_LEFT (ID_SPRITE_MARIO_BIG_SIT + 10)
#define ID_SPRITE_MARIO_BIG_SIT_RIGHT (ID_SPRITE_MARIO_BIG_SIT + 20)

#define ID_SPRITE_MARIO_BIG_BRACE (ID_SPRITE_MARIO_BIG + 600)
#define ID_SPRITE_MARIO_BIG_BRACE_LEFT (ID_SPRITE_MARIO_BIG_BRACE + 10)
#define ID_SPRITE_MARIO_BIG_BRACE_RIGHT (ID_SPRITE_MARIO_BIG_BRACE + 20)

#define ID_SPRITE_MARIO_BIG_KICK (ID_SPRITE_MARIO_BIG + 700)
#define ID_SPRITE_MARIO_BIG_KICK_LEFT (ID_SPRITE_MARIO_BIG_KICK + 10)
#define ID_SPRITE_MARIO_BIG_KICK_RIGHT (ID_SPRITE_MARIO_BIG_KICK + 20)

#define ID_SPRITE_MARIO_BIG_HOLD (ID_SPRITE_MARIO_BIG + 800)
#define ID_SPRITE_MARIO_BIG_HOLD_LEFT (ID_SPRITE_MARIO_BIG_HOLD + 10)
#define ID_SPRITE_MARIO_BIG_HOLD_RIGHT (ID_SPRITE_MARIO_BIG_HOLD + 20)

#define ID_SPRITE_MARIO_BIG_JUMP_RUN (ID_SPRITE_MARIO_BIG + 900)
#define ID_SPRITE_MARIO_BIG_JUMP_RUN_LEFT (ID_SPRITE_MARIO_BIG_JUMP_WALK + 10)
#define ID_SPRITE_MARIO_BIG_JUMP_RUN_RIGHT (ID_SPRITE_MARIO_BIG_JUMP_WALK + 20)
#pragma endregion

#pragma region MARIO_SMALL

#define ID_SPRITE_MARIO_SMALL (ID_SPRITE_MARIO + 2000)

#define ID_SPRITE_MARIO_SMALL_IDLE (ID_SPRITE_MARIO_SMALL + 100)
#define ID_SPRITE_MARIO_SMALL_IDLE_LEFT (ID_SPRITE_MARIO_SMALL_IDLE + 10)
#define ID_SPRITE_MARIO_SMALL_IDLE_RIGHT (ID_SPRITE_MARIO_SMALL_IDLE + 20)

#define ID_SPRITE_MARIO_SMALL_WALKING (ID_SPRITE_MARIO_SMALL + 200)
#define ID_SPRITE_MARIO_SMALL_WALKING_LEFT (ID_SPRITE_MARIO_SMALL_WALKING + 10)
#define ID_SPRITE_MARIO_SMALL_WALKING_RIGHT (ID_SPRITE_MARIO_SMALL_WALKING + 20)

#define ID_SPRITE_MARIO_SMALL_RUNNING (ID_SPRITE_MARIO_SMALL + 300)
#define ID_SPRITE_MARIO_SMALL_RUNNING_LEFT (ID_SPRITE_MARIO_SMALL_RUNNING + 10)
#define ID_SPRITE_MARIO_SMALL_RUNNING_RIGHT (ID_SPRITE_MARIO_SMALL_RUNNING + 20)

#define ID_SPRITE_MARIO_SMALL_JUMP_WALK (ID_SPRITE_MARIO_SMALL + 400)
#define ID_SPRITE_MARIO_SMALL_JUMP_WALK_LEFT (ID_SPRITE_MARIO_SMALL_JUMP_WALK + 10)
#define ID_SPRITE_MARIO_SMALL_JUMP_WALK_RIGHT (ID_SPRITE_MARIO_SMALL_JUMP_WALK + 20)

#define ID_SPRITE_MARIO_SMALL_SIT (ID_SPRITE_MARIO_SMALL + 500)
#define ID_SPRITE_MARIO_SMALL_SIT_LEFT (ID_SPRITE_MARIO_SMALL_SIT + 10)
#define ID_SPRITE_MARIO_SMALL_SIT_RIGHT (ID_SPRITE_MARIO_SMALL_SIT + 20)

#define ID_SPRITE_MARIO_SMALL_BRACE (ID_SPRITE_MARIO_SMALL + 600)
#define ID_SPRITE_MARIO_SMALL_BRACE_LEFT (ID_SPRITE_MARIO_SMALL_BRACE + 10)
#define ID_SPRITE_MARIO_SMALL_BRACE_RIGHT (ID_SPRITE_MARIO_SMALL_BRACE + 20)

#define ID_SPRITE_MARIO_SMALL_KICK (ID_SPRITE_MARIO_SMALL + 700)
#define ID_SPRITE_MARIO_SMALL_KICK_LEFT (ID_SPRITE_MARIO_SMALL_KICK + 10)
#define ID_SPRITE_MARIO_SMALL_KICK_RIGHT (ID_SPRITE_MARIO_SMALL_KICK + 20)

#define ID_SPRITE_MARIO_SMALL_HOLD (ID_SPRITE_MARIO_SMALL + 800)
#define ID_SPRITE_MARIO_SMALL_HOLD_LEFT (ID_SPRITE_MARIO_SMALL_HOLD + 10)
#define ID_SPRITE_MARIO_SMALL_HOLD_RIGHT (ID_SPRITE_MARIO_SMALL_HOLD + 20)

#define ID_SPRITE_MARIO_SMALL_JUMP_RUN (ID_SPRITE_MARIO_SMALL + 900)
#define ID_SPRITE_MARIO_SMALL_JUMP_RUN_LEFT (ID_SPRITE_MARIO_SMALL_JUMP_RUN + 10)
#define ID_SPRITE_MARIO_SMALL_JUMP_RUN_RIGHT (ID_SPRITE_MARIO_SMALL_JUMP_RUN + 20)

#pragma endregion

#pragma region MARIO_RACCOON

#define ID_SPRITE_MARIO_RACCOON (ID_SPRITE_MARIO + 3000)

#define ID_SPRITE_MARIO_RACCOON_IDLE (ID_SPRITE_MARIO_RACCOON + 100)
#define ID_SPRITE_MARIO_RACCOON_IDLE_LEFT (ID_SPRITE_MARIO_RACCOON_IDLE + 10)
#define ID_SPRITE_MARIO_RACCOON_IDLE_RIGHT (ID_SPRITE_MARIO_RACCOON_IDLE + 20)

#define ID_SPRITE_MARIO_RACCOON_WALKING (ID_SPRITE_MARIO_RACCOON + 200)
#define ID_SPRITE_MARIO_RACCOON_WALKING_LEFT (ID_SPRITE_MARIO_RACCOON_WALKING + 10)
#define ID_SPRITE_MARIO_RACCOON_WALKING_RIGHT (ID_SPRITE_MARIO_RACCOON_WALKING + 20)

#define ID_SPRITE_MARIO_RACCOON_RUNNING (ID_SPRITE_MARIO_RACCOON + 300)
#define ID_SPRITE_MARIO_RACCOON_RUNNING_LEFT (ID_SPRITE_MARIO_RACCOON_RUNNING + 10)
#define ID_SPRITE_MARIO_RACCOON_RUNNING_RIGHT (ID_SPRITE_MARIO_RACCOON_RUNNING + 20)

#define ID_SPRITE_MARIO_RACCOON_JUMP (ID_SPRITE_MARIO_RACCOON + 400)
#define ID_SPRITE_MARIO_RACCOON_JUMP_WALK (ID_SPRITE_MARIO_RACCOON_JUMP + 10)
#define ID_SPRITE_MARIO_RACCOON_JUMP_WALK_LEFT (ID_SPRITE_MARIO_RACCOON_JUMP_WALK + 2)
#define ID_SPRITE_MARIO_RACCOON_JUMP_WALK_RIGHT (ID_SPRITE_MARIO_RACCOON_JUMP_WALK + 6)
#define ID_SPRITE_MARIO_RACCOON_JUMP_RUN (ID_SPRITE_MARIO_RACCOON_JUMP + 20)
#define ID_SPRITE_MARIO_RACCOON_JUMP_RUN_LEFT (ID_SPRITE_MARIO_RACCOON_JUMP_RUN + 2)
#define ID_SPRITE_MARIO_RACCOON_JUMP_RUN_RIGHT (ID_SPRITE_MARIO_RACCOON_JUMP_RUN + 6)

#define ID_SPRITE_MARIO_RACCOON_SIT (ID_SPRITE_MARIO_RACCOON + 500)
#define ID_SPRITE_MARIO_RACCOON_SIT_LEFT (ID_SPRITE_MARIO_RACCOON_SIT + 10)
#define ID_SPRITE_MARIO_RACCOON_SIT_RIGHT (ID_SPRITE_MARIO_RACCOON_SIT + 20)

#define ID_SPRITE_MARIO_RACCOON_BRACE (ID_SPRITE_MARIO_RACCOON + 600)
#define ID_SPRITE_MARIO_RACCOON_BRACE_LEFT (ID_SPRITE_MARIO_RACCOON_BRACE + 10)
#define ID_SPRITE_MARIO_RACCOON_BRACE_RIGHT (ID_SPRITE_MARIO_RACCOON_BRACE + 20)


#pragma endregion


#define ID_SPRITE_MARIO_DIE (ID_SPRITE_MARIO + 19999)

#pragma endregion 

// BLOCK includes bricks, cloud block, P_Block that transfer bricks to coins,...
// basically anything that's like a block
#pragma region BLOCK

#define ID_SPRITE_BLOCK 30000

#define ID_SPRITE_BLOCK_BRICK (ID_SPRITE_BLOCK + 1000)

#define ID_SPRITE_BLOCK_BRICK_QUESTIONMARK (ID_SPRITE_BLOCK_BRICK + 100)	
#define ID_SPRITE_BLOCK_BRICK_QUESTIONMARK_NORMAL (ID_SPRITE_BLOCK_BRICK_QUESTIONMARK + 10)	
#define ID_SPRITE_BLOCK_BRICK_QUESTIONMARK_HIT (ID_SPRITE_BLOCK_BRICK_QUESTIONMARK + 20)	

#define ID_SPRITE_BLOCK_BRICK_GLASS (ID_SPRITE_BLOCK_BRICK + 200)

#define ID_SPRITE_BLOCK_PBLOCK (ID_SPRITE_BLOCK + 2000)

#pragma endregion

// ITEM includes items that give powerups, and items that give u scores and money (like coins)
#pragma region ITEM

#define ID_SPRITE_ITEM 40000

#define ID_SPRITE_ITEM_POWERUP (ID_SPRITE_ITEM + 1000)
#define ID_SPRITE_ITEM_POWERUP_MUSHROOM_BIG (ID_SPRITE_ITEM_POWERUP + 100)
#define ID_SPRITE_ITEM_POWERUP_MUSHROOM_1UP (ID_SPRITE_ITEM_POWERUP + 200)

#define ID_SPRITE_ITEM_POWERUP_LEAF (ID_SPRITE_ITEM_POWERUP + 300)
#define ID_SPRITE_ITEM_POWERUP_LEAF_MOVING_LEFT (ID_SPRITE_ITEM_POWERUP_LEAF + 10)
#define ID_SPRITE_ITEM_POWERUP_LEAF_MOVING_RIGHT (ID_SPRITE_ITEM_POWERUP_LEAF + 20)
							  
#define ID_SPRITE_ITEM_POWERUP_FIREFLOWER (ID_SPRITE_ITEM_POWERUP + 400)

#define ID_SPRITE_ITEM_ACHIEVEMENTS (ID_SPRITE_ITEM + 4000)
#define ID_SPRITE_ITEM_ACHIEVEMENTS_COINS (ID_SPRITE_ITEM_ACHIEVEMENTS + 100)

#pragma endregion

#pragma region ENEMY

#define ID_SPRITE_ENEMY 50000

#define ID_SPRITE_ENEMY_GOOMBA (ID_SPRITE_ENEMY + 1000)
#define ID_SPRITE_ENEMY_GOOMBA_BROWN_NORMAL (ID_SPRITE_ENEMY_GOOMBA + 100)
#define ID_SPRITE_ENEMY_GOOMBA_BROWN_NORMAL_WALK (ID_SPRITE_ENEMY_GOOMBA_BROWN_NORMAL + 10)
#define ID_SPRITE_ENEMY_GOOMBA_BROWN_NORMAL_DIE (ID_SPRITE_ENEMY_GOOMBA_BROWN_NORMAL + 20)


#define ID_SPRITE_ENEMY_GOOMBA_BROWN_WING (ID_SPRITE_ENEMY_GOOMBA + 200)
#define ID_SPRITE_ENEMY_GOOMBA_BROWN_WING_WALK (ID_SPRITE_ENEMY_GOOMBA_BROWN_WING + 10)
#define ID_SPRITE_ENEMY_GOOMBA_BROWN_WING_DIE (ID_SPRITE_ENEMY_GOOMBA_BROWN_WING + 20)

#define ID_SPRITE_ENEMY_GOOMBA_RED_NORMAL (ID_SPRITE_ENEMY_GOOMBA + 300)
#define ID_SPRITE_ENEMY_GOOMBA_RED_NORMAL_WALK (ID_SPRITE_ENEMY_GOOMBA_RED_NORMAL + 10)
#define ID_SPRITE_ENEMY_GOOMBA_RED_NORMAL_DIE (ID_SPRITE_ENEMY_GOOMBA_RED_NORMAL + 20)

#define ID_SPRITE_ENEMY_GOOMBA_RED_WING (ID_SPRITE_ENEMY_GOOMBA + 400)
#define ID_SPRITE_ENEMY_GOOMBA_RED_WING_WALK (ID_SPRITE_ENEMY_GOOMBA_RED_WING + 10)
#define ID_SPRITE_ENEMY_GOOMBA_RED_WING_DIE (ID_SPRITE_ENEMY_GOOMBA_RED_WING + 20)


#define ID_SPRITE_ENEMY_KOOPA (ID_SPRITE_ENEMY + 2000)
#define ID_SPRITE_ENEMY_KOOPA_GREEN_NORMAL (ID_SPRITE_ENEMY_KOOPA + 100)
#define ID_SPRITE_ENEMY_KOOPA_GREEN_WING (ID_SPRITE_ENEMY_KOOPA + 200)
#define ID_SPRITE_ENEMY_KOOPA_RED_NORMAL (ID_SPRITE_ENEMY_KOOPA + 300)
#define ID_SPRITE_ENEMY_KOOPA_RED_WING (ID_SPRITE_ENEMY_KOOPA + 400)

#define ID_SPRITE_ENEMY_PLANT (ID_SPRITE_ENEMY + 3000)
#define ID_SPRITE_ENEMY_PLANT_RED_NORMAL (ID_SPRITE_ENEMY_PLANT + 100)
#define ID_SPRITE_ENEMY_PLANT_RED_FIRE (ID_SPRITE_ENEMY_PLANT + 200)
#define ID_SPRITE_ENEMY_PLANT_GREEN_NORMAL (ID_SPRITE_ENEMY_PLANT + 300)
#define ID_SPRITE_ENEMY_PLANT_GREEN_FIRE (ID_SPRITE_ENEMY_PLANT + 400)

#pragma endregion

//#define ID_SPRITE_GOOMBA 30000
//#define ID_SPRITE_GOOMBA_WALK (ID_SPRITE_GOOMBA + 1000)
//#define ID_SPRITE_GOOMBA_DIE (ID_SPRITE_GOOMBA + 2000)
//
//#define ID_SPRITE_COIN 40000

//
//#define ID_SPRITE_CLOUD 50000
//#define ID_SPRITE_CLOUD_BEGIN (ID_SPRITE_CLOUD+1000)
//#define ID_SPRITE_CLOUD_MIDDLE (ID_SPRITE_CLOUD+2000)
//#define ID_SPRITE_CLOUD_END (ID_SPRITE_CLOUD+3000)