#include <iostream>
#include <string>
#include <vector>
#include "map.h"

using namespace std;

bool Map::ReadFromStdin() {
  height_ = 0;

  string line;
  vector<string> ascii_map;
  while (getline(cin, line)) {
    if (line.length() == 0) {
      // empty line -> end of map; start parsing metadata, if any
      break;
    }
    width_ = max(width_, (int) line.length());
    ascii_map.push_back(line);
  }

  size_t trampoline_index = 0, target_index = 0;
  // c++11 style would be: for(const string& line : ascii_map) {
  for (size_t j=0; j<ascii_map.size(); j++) {
    const string& line = ascii_map[j];
    for (unsigned i=0; i<line.length(); i++) {
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
        case 'A':   // trampoline (teleport entrance)
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
          setTrampoline(trampoline_index, map_.size()); 
          map_.push_back(TRAMPOLINE);
          trampoline_index = line[i] - 'A';
          cerr << "Setting trampoline " << map_.size() << " to " << target_index << endl;
          break;
        case '1':    // target (teleport exit)
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          setTarget(target_index, map_.size()); 
          map_.push_back(TARGET);
          target_index = line[i] - '1';
          cerr << "Setting target " << map_.size() << " to " << target_index << endl;
          break;
        default:
          cerr << "Unrecognized map character " << line[i] << endl;
          return false;
      }
    }

    // Right-fill any short lines (e.g. in maps/flood4.map)
    for (unsigned int k=0; k<width_ - line.length(); k++)
      map_.push_back(EMPTY);

    height_ ++;
  }
  return true;
}

ostream& operator<<(ostream& ostr, const Map& m) {
  for (int j=0; j<m.height(); j++) {
    for (int i=0; i<m.width(); i++) {
      if (m.robot_x() == i && m.robot_y() == j)
        ostr << 'R';
      else {
        Terrain terrain = m.terrain(i,j);
        switch (terrain) {
          case TARGET:
            ostr << (char) ('1' + m.getTarget(i,j));
            break;
          case TRAMPOLINE:
            ostr << (char) ('A' + m.getTrampoline(i,j));
            break;
          default:
            ostr << TerrainChar[static_cast<int>(terrain)];
            break;
        }
      }
    }
    ostr << endl;
  }
  return ostr;
}

bool State::ReadFromStdin() {
  map_.ReadFromStdin();
  string line;
  string value;
  while(cin >> line >> value) {
    switch (line.size()) {
      case 5:   //Water
        init_water_ = atoi(value);
        break;
      case 8:   //Flooding
        flood_rate_ = atoi(value);
        break;
      case 10:  //Waterproof or Trampoline
        if (line[0] == 'W') {
          water_proof_ = atoi(value);
        } else {
          int trampoline = value[0] - 'A';
          cin >> line >> value;
          int target = value[0] - '1';
          map_.setLinkage(trampoline, target);
        }
        break;
    }
  }
  return true;
}

LifeStatus State::MakeMove(Move move) {
  Delta delta;
  delta.old_water_count_ = water_count_;
  bool rock_fell = map_.MakeMove(move, &delta);

  history_.push_back(delta);
  collected_lambdas_ += (delta.old_terrain_ == LAMBDA) ? 1 : 0;
  turn_ ++;
  if ((map_.height() - map_.robot_y()) > init_water_ + (turn_ / flood_rate_)) {
    water_count_ ++;
  } else {
    water_count_ = 0;
  }


  if (delta.old_terrain_ == EXIT) {
    return EXITED;
  } else if (move == ABORT) {
    return ABORTED;
  } else if (rock_fell || water_count_ > water_proof_) {
    return DIED;
  } else {
    return ALIVE;
  }
}

void State::Rollback() {
  Delta& delta = history_.back();
  turn_--;
  water_count_ = delta.old_water_count_;
  collected_lambdas_ -= (delta.old_terrain_ == LAMBDA) ? 1 : 0;

  map_.Rollback(delta);
  history_.pop_back();
}



bool Map::MakeMove(Move move, Delta* delta) {
  ResolvedMove move_r = ResolveMove(move);
  bool rock_fell = DoResolvedMove(move_r, delta);

  return rock_fell;
}

bool Map::DoResolvedMove(ResolvedMove move, Delta* delta) {
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

void Map::Rollback(const Delta& delta) {
  vector<int>::const_iterator itr;
  for (itr = delta.new_rocks_.begin(); itr != delta.new_rocks_.end(); itr++) {
    map_[*itr] = EMPTY;
  }
  for (itr = delta.old_rocks_.begin(); itr != delta.old_rocks_.end(); itr++) {
    map_[*itr] = ROCK;
  }

  terrain(robot_x_,robot_y_) = delta.old_terrain_;
  if (delta.move_ == PUSH_RIGHT_R || delta.move_ == PUSH_LEFT_R) {
    terrain(robot_x_ + (delta.move_ == PUSH_RIGHT_R ? 1 : -1), robot_y_) = EMPTY;
  }
  robot_x_ = delta.old_robot_x_;
  robot_y_ = delta.old_robot_y_;
}


bool Map::Update(Delta *delta) {
  //Updating from right to left is OK because it is equivalent
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
        delta->old_rocks_.push_back(i);
        delta->new_rocks_.push_back(down_index);
      } else if (down == ROCK) {
        if (right == EMPTY && right_down == EMPTY) {
          delta->old_rocks_.push_back(i);
          delta->new_rocks_.push_back(right_down_index);
        } else if (left == EMPTY && left_down == EMPTY) {
          delta->old_rocks_.push_back(i);
          delta->new_rocks_.push_back(left_down_index);
        }
      } else if (down == LAMBDA) {
        if (right == EMPTY && right_down == EMPTY) {
          delta->old_rocks_.push_back(i);
          delta->new_rocks_.push_back(right_down_index);
        }
      }
    }
  }

  bool rock_fell_on_head = false;
  vector<int>::const_iterator itr;
  for (itr = delta->new_rocks_.begin() ; itr != delta->new_rocks_.end(); itr++) {
    map_[*itr] = ROCK;
    if (*itr + width_ == (robot_y_ * width_ + robot_x_))
      rock_fell_on_head = true;
  }
  for (itr = delta->old_rocks_.begin() ; itr != delta->old_rocks_.end(); itr++) {
    map_[*itr] = EMPTY;
  }

  return rock_fell_on_head;
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
    case TARGET:
    case TRAMPOLINE:
      //TODO implement
      return WAIT_R;
  }

  //Should never happen
  return WAIT_R;
}
