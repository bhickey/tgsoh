#ifndef __TGSOH_ENUMS_HEADER__
#define __TGSOH_ENUMS_HEADER__

enum Move { LEFT, RIGHT, UP, DOWN, WAIT, ABORT };
enum ResolvedMove { LEFT_R, RIGHT_R, UP_R, DOWN_R, WAIT_R, PUSH_LEFT_R, PUSH_RIGHT_R };
enum RockFall {FALL_DOWN, FALL_RIGHT, FALL_LEFT, FALL_LEFT_RIGHT};
enum Terrain { AIR, EARTH, EXIT, LAMBDA, ROCK, WALL };

#endif /* __TGSOH_ENUMS_HEADER__ */
