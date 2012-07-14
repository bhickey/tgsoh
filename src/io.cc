#ifndef __TGSOH_IO_HEADER__
#define __TGSOH_IO_HEADER__

#include <vector>
#include <iostream>
#include "enums.h"

char MoveNames[] = {'L', 'R', 'U', 'D', 'W', 'A'};

std::ostream& operator<<(std::ostream& o, const Move &move)
{
  o << MoveNames[(int)move];
  return o;
}

std::ostream& operator<<(std::ostream& o, std::vector<Move> &moves)
{
  std::vector<Move>::iterator itr;
  for (itr = moves.begin() ; itr != moves.end(); itr++) {
    o << *itr;
  }
  return o;
}

char atoi(const std::string str) {
  return atoi(str.c_str());
}

#endif /* __TGSOH_IO_HEADER__ */
