#ifndef __TGSOH_MAP_HEADER__
#define __TGSOH_MAP_HEADER__

#include <map>
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
    linkages_ = std::vector<int>(9, -1);
    targets_ = std::vector<int>(9, 0 );
    targets_loc_ = std::map<int, int>();
    trampolines_ = std::vector<int>(9, 0);
    trampolines_loc_ = std::map<int, int>();
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

  void setTrampoline(int trampoline, int location) {
    trampolines_[trampoline] = location;
    trampolines_loc_.insert(std::pair<int, int>(location, trampoline));
    if (trampolines_loc_.find(location)->second != trampoline) {
      std::cerr << "fuck";
    }
  }

  int getTrampoline(const int x, const int y) const {
    return trampolines_loc_.find(x+y*width_)->second;
  }

  void setTarget(const int target, const int location) {
    targets_[target] = location;
    targets_loc_.insert(std::pair<int, int>(location, target));
  }

  int getTarget(const int x, const int y) const {
    return targets_loc_.find(x+y*width_)->second;
  }

  void setLinkage(const int from, const int to) {
    linkages_[from] = to;
  }

 private:
  int width_, height_;
  int robot_x_, robot_y_;
  int remaining_lambdas_;
  std::vector<int> trampolines_, targets_, linkages_;
  std::map<int,int> trampolines_loc_, targets_loc_;
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

