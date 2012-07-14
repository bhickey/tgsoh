#ifndef __TGSOH_MAP_HEADER__
#define __TGSOH_MAP_HEADER__

#include <vector>
#include "enums.h"

class Map
{
 public:
  Map() { Init(); }
  ~Map() { }

  void Init() {
    width_ = height_ = 0;
    robot_x_ = robot_y_ = 0;
  }

  bool ReadFromStdin();

 private:
  int width_, height_;
  int robot_x_, robot_y_;
  std::vector<Terrain> map_;
};

#endif /* __TGSOH_MAP_HEADER__ */

