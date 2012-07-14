#include <iostream>
#include <string>
#include "map.h"

using namespace std;

bool Map::ReadFromStdin() {
  height_ = 0;

  string line;
  while (getline(cin, line)) {
    if (line.size() <= 1) {
      // empty line -> end of map; start parsing metadata, if any
      break;
    }
    if (width_ == 0) width_ = line.length();
    // cout << "line " << height_ << " size " << line.length() << " " << line << endl;

    for (unsigned i=0; i<line.size(); i++) {
      switch (line[i]) {
        case '#':   // Wall
          map_.push_back(WALL);
          break;
        case ' ':   // air
          map_.push_back(EMPTY);
          break;
        case '.':   // earth
          map_.push_back(EARTH);
          break;
        case '\\':  // lambda
          map_.push_back(LAMBDA);
          remaining_lambdas_ ++;
          break;
        case '*':   // rock
          map_.push_back(ROCK);
          break;
        case 'R':   // robot location (air underneath)
          map_.push_back(EMPTY);
          robot_x_ = i;
          robot_y_ = height_;
          break;
        case 'L':   // exit (lambda lift)
          map_.push_back(EXIT);
          break;
        default:
          cerr << "Unrecognized map character " << line[i] << endl;
          return false;
      }
    }
    height_ ++;
  }
  return true;
}

ostream& operator<<(ostream& ostr, const Map& m) {
  for (int j=0; j<m.height_; j++) {
    for (int i=0; i<m.width_; i++) {
      if (m.robot_x_ == i && m.robot_y_ == j)
        ostr << 'R';
      else
        ostr << TerrainChar[static_cast<int>(m.terrain(i,j))];
    }
    ostr << endl;
  }
  return ostr;
}

bool State::ReadFromStdin() {
  map_.ReadFromStdin();
  string line;
  int value;
  while(cin >> line >> value) {
    switch (line.size()) {
      case 5:   //Water
        water_ = value;
        break;
      case 8:   //Flooding
        flood_rate_ = value;
        break;
      case 10:  //Waterproof
        water_proof_ = value;
        break;
    }
  }
  return true;
}


bool Map::MakeMove(Move move, Delta *delta) {
  ResolvedMove move_r = ResolveMove(move);
  bool rock_fell_on_head = DoResolvedMove(move_r, delta);

  return true; //TODO
}

bool Map::DoResolvedMove(ResolvedMove move, Delta *delta) {
  int new_x = robot_x_, new_y = robot_y_;
  int new_rock_x = robot_x_, new_rock_y = robot_y_;
  bool push = false;
  switch (move) {
    case PUSH_LEFT_R:
      push = true;
      new_rock_x -= 2;
      // fallthrough
    case LEFT_R:
      new_x -= 1;
      break;
    case PUSH_RIGHT_R:
      push = true;
      new_rock_x += 2;
      // fallthrough
    case RIGHT_R:
      new_x += 1;
      break;
    case UP_R:
      new_y -= 1;
      break;
    case DOWN_R:
      new_y += 1;
      break;
    case WAIT_R:
      // Do Nothing.
      break;
  }

  //Copy old locations
  //rocks are set in update
  delta->old_terrain_ = terrain(new_x, new_y);
  delta->old_robot_x_ = robot_x_;
  delta->old_robot_y_ = robot_y_;
  delta->move_ = move;

  if (terrain(new_x, new_y) != EXIT)
    terrain(new_x, new_y) = EMPTY;
  if (push)
    terrain(new_rock_x, new_rock_y) = ROCK;
  robot_x_ = new_x;
  robot_y_ = new_y;

  return Update(delta);
}

bool Map::Update(Delta *delta) {
  //Updating from right to left is OK because it is equivalent
  vector<Terrain> new_map(map_);

  for (int i = width_ * height_ - 1; i >= 0; i--) {
    if (map_[i] == ROCK) {
      int left_index = i-1;
      int right_index = i+1;
      int left_down_index = i-1+width_;
      int down_index = i+width_;
      int right_down_index = i+1+width_;
      Terrain left = map_[left_index];
      Terrain right = map_[right_index];
      Terrain left_down = map_[left_down_index];
      Terrain down = map_[down_index];
      Terrain right_down = map_[right_down_index];

      if (down == EMPTY) {
        new_map[i] = EMPTY;
        new_map[down_index] = ROCK;
      } else if (down == ROCK) {
        if (right == EMPTY && right_down == EMPTY) {
          new_map[i] = EMPTY;
          new_map[right_down_index] = ROCK;
        } else if (left == EMPTY && left_down == EMPTY) {
          new_map[i] = EMPTY;
          new_map[left_down_index] = ROCK;
        }
      } else if (down == LAMBDA) {
        if (right == EMPTY && right_down == EMPTY) {
          new_map[i] = EMPTY;
          new_map[right_down_index] = ROCK;
        }
      }
    }
  }
  //TODO add the rocks to delta


  //TODO avoid the copy here
  map_ = new_map;

  //TODO Did a rock fall on your head?
  return false;
}

ResolvedMove Map::ResolveMove(Move move) {
  if (move == WAIT || move == ABORT)
    return WAIT_R;

  int change_x = MoveDX[static_cast<int>(move)],
      change_y = MoveDY[static_cast<int>(move)];

  int new_x = robot_x_ + change_x;
  int new_y = robot_y_ + change_y;

  Terrain new_space = terrain(new_x,new_y);

  switch (new_space) {
    case EXIT:
      if (remaining_lambdas_ != 0) {
        return WAIT_R;
      }
      //fall through
    case EMPTY:
    case LAMBDA:
    case EARTH:
      switch (move) {
        case LEFT:
          return LEFT_R;
        case RIGHT:
          return RIGHT_R;
        case UP:
          return UP_R;
        case DOWN:
          return DOWN_R;
        case WAIT:
        case ABORT:
          //Should never happen
          return WAIT_R;
      }
    case WALL:
      return WAIT_R;
    case ROCK:
      switch (move) {
        case UP:
        case DOWN:
          return WAIT_R;
        case WAIT:
        case ABORT:
          //Should never happen
          return WAIT_R;
        case LEFT:
        case RIGHT:
          int new_rock_x = new_x + change_x;
          int new_rock_y = new_y;
          bool can_push = terrain(new_rock_x, new_rock_y) == EMPTY;
          if (can_push) {
            return (move == LEFT) ? PUSH_LEFT_R : PUSH_RIGHT_R;
          } else {
            return WAIT_R;
          }
      }
  }

  //Should never happen
  return WAIT_R;
}
