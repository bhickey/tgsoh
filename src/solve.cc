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
  cerr << map.width() << "x" << map.height() << endl;
  cerr << map;

  while (true)
  {
    // spin wheels
  }

  return 0;
}

