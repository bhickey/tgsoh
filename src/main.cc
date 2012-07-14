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
  while(true) {
   // do the stuff that matters
  }
}
