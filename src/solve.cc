#include <iostream>
#include "map.h"

using namespace std;

int main()
{
  Map map;
  if (!map.ReadFromStdin()) {
    cerr << "can't read map\n";
    cout << "A\n";
    return -1;
  }
  cerr << map.width() << "x" << map.height() << endl;
}

