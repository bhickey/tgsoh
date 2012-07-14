#ifndef __TGSOH_ENUMS_HEADER__
#define __TGSOH_ENUMS_HEADER__

enum Move { LEFT = 0, RIGHT, UP, DOWN, WAIT, ABORT };
enum ResolvedMove { LEFT_R, RIGHT_R, UP_R, DOWN_R, WAIT_R, PUSH_LEFT_R, PUSH_RIGHT_R };
enum RockFall {FALL_DOWN, FALL_RIGHT, FALL_LEFT, FALL_LEFT_RIGHT};
enum Terrain { EARTH = 0, EMPTY, EXIT, LAMBDA, ROCK, WALL };

static const int MoveDX[] = { -1, 1, 0, 0, 0, 0 };
static const int MoveDY[] = { 0, 0, -1, 1, 0, 0 };

static const char TerrainChar[] = { '.', ' ', 'X', '\\', '*', '#' };

#endif /* __TGSOH_ENUMS_HEADER__ */
