 /*
  * main.cpp
  *
  * Created on: 20 Feb 2017
  * Author    : Noni Hua
  *
  * Brief     : Main function that runs markless augmented reality
  *             Will call function from a finite-state machine and execute
  *             Scheme: Detections -> Tracking -> Displaying
  */

#include <stdio.h>
#include <stdlib.h>
#include "Detections.cpp"

using namespace std;
using namespace opencv_handler;

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    cout << "Usage: node <video input>" << endl;
    return -1;
  }

  /* create image processing instance*/
  Detections detector;
  detector.imgStreamIn(atoi(argv[1]));
}
