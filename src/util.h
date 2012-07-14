#ifndef __TGSOH_UTIL_HEADER__
#define __TGSOH_UTIL_HEADER__

#include <vector>
#include <iostream>
#include "enums.h"

std::ostream& operator<<(std::ostream& o, const Move &move);

std::ostream& operator<<(std::ostream& o, std::vector<Move> &moves);

char atoi(const std::string str);

#endif /* __TGSOH_UTIL_HEADER__ */
