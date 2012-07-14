#include <iostream>
#include <string>
#include "map.h"

using namespace std;

bool Map::ReadFromStdin() {
  height_ = 0; 

  string line;
  while(cin >> line) {
    if (line.size() == 0) {
      // empty line -> end of map; start parsing metadata, if any
      break;
    }
    width_ = line.size();

    for (unsigned i=0; i<line.size(); i++) {
      switch (line[i]) {
        case '#':   // Wall
          map_.push_back(WALL);
          break;
        case ' ':   // air
          map_.push_back(AIR);
          break;
        case '.':   // earth
          map_.push_back(EARTH);
          break;
        case '\\':  // lambda
          map_.push_back(LAMBDA);
          break;
        case '*':   // rock
          map_.push_back(ROCK);
          break;
        case 'R':   // robot location (air underneath)
          map_.push_back(AIR);
          robot_x_ = i;
          robot_y_ = height_;
          break;
        case 'L':   // exit (lambda lift)
          map_.push_back(EXIT);
          break;
      }
    }
    height_ ++;
  }
  // TODO: flooding metadata
  //while(cin >> line) {
  //}
  return true;
}

