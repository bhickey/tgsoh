#ifndef __TGSOH_IO_HEADER__
#define __TGSOH_IO_HEADER__

#include <vector>
#include <iostream>
#include "enums.h"

std::ostream& operator<<(std::ostream& o, const Move &move);

std::ostream& operator<<(std::ostream& o, std::vector<Move> &moves);

char atoi(const std::string str);

#endif /* __TGSOH_IO_HEADER__ */
