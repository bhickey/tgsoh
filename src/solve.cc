#include <iostream>
#include "map.h"

using namespace std;

void output (int param) {
  cout << "A";
  exit(0);
}

int main()
{
  signal(SIGINT, output);
  Map map;
  if (!map.ReadFromStdin()) {
    cerr << "can't read map\n";
    cout << "A\n";
    return -1;
  }
  cerr << map.width_ << "x" << map.height_ << endl;
  cerr << map;

  Delta d;
  map.MakeMove(RIGHT, &d);
  cerr << map;
  map.MakeMove(RIGHT, &d);
  cerr << map;
  map.MakeMove(RIGHT, &d);
  cerr << map;

#if 0
  while(true) {
   // do the stuff that matters
  }
#endif
  return 0;
}
