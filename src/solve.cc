#include <iostream>
#include "enums.h"
#include "map.h"
#include "util.h"

using namespace std;

void output (int param) {
  vector<Move> moves;
  moves.push_back(ABORT);
  cout << moves;
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
  cerr << map.width() << "x" << map.height() << endl;
  cerr << map;

  Delta d;
  map.MakeMove(RIGHT, &d);
  cerr << map;
  map.MakeMove(RIGHT, &d);
  cerr << map;
  map.MakeMove(RIGHT, &d);
  cerr << map;

#if 1
  while(true) {
   // do the stuff that matters
  }
#endif
  return 0;
}
