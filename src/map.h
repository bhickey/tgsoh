#ifndef __TGSOH_MAP_HEADER__
#define __TGSOH_MAP_HEADER__

#include <vector>
#include <iostream>
#include "enums.h"
#include "util.h"

class Delta
{
 public:
  Delta() { Init(); }
  ~Delta() { }

  void Init() {
    old_terrain_ = EMPTY;
    old_water_count_ = 0;
    move_ = WAIT_R;
    old_robot_x_ = old_robot_y_ = 0;
  }

 Terrain old_terrain_;
 std::vector<int> new_rocks_;
 std::vector<int> old_rocks_;
 int old_water_count_;
 int old_robot_x_, old_robot_y_;
 ResolvedMove move_;

 private:
};

class Map
{
 public:
  Map() { Init(); }
  ~Map() { }

  void Init() {
    width_ = height_ = 0;
    robot_x_ = robot_y_ = 0;
    remaining_lambdas_ = 0;
  }

  const Terrain& terrain(int x, int y) const { return map_[x+y*width_]; }
  Terrain& terrain(int x, int y) { return map_[x+y*width_]; }
  const Terrain& operator()(int x, int y) const { return map_[x+y*width_]; }
  Terrain& operator()(int x, int y) { return map_[x+y*width_]; }

  bool ReadFromStdin();

  bool MakeMove(Move move, Delta *delta);
  ResolvedMove ResolveMove(Move move);
  void Rollback(const Delta& delta);

  int width() const { return width_; }
  int height() const { return height_; }
  int robot_x() const { return robot_x_; }
  int robot_y() const { return robot_y_; }

 private:
  int width_, height_;
  int robot_x_, robot_y_;
  int remaining_lambdas_;

 private:
  std::vector<Terrain> map_;

  bool DoResolvedMove(ResolvedMove move, Delta *delta);
  bool Update(Delta *delta);
};

extern std::ostream& operator<<(std::ostream& ostr, const Map& m);

class State
{
 public:
  State() { Init(); }
  ~State() { }

  void Init() {
    turn_ = flood_rate_ = init_water_ = water_proof_ = water_count_ = 0;
    collected_lambdas_ = 0;
  }

  bool ReadFromStdin();
  void Rollback();
  LifeStatus MakeMove(Move move);

 private:
  Map map_;
  int turn_, flood_rate_, init_water_, water_proof_, water_count_;
  int collected_lambdas_;
  std::vector<Delta> history_;
};


class Walk
{
  Walk() { Init(); }
  ~Walk() { }

  void Init() {
  }

  std::vector<Move> steps;

};


#endif /* __TGSOH_MAP_HEADER__ */

