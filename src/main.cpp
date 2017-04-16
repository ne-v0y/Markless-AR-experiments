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
#include "Tracking.cpp"

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
  Detections mydetector;
  Tracking mytracker;

  mydetector.imgStreamIn(atoi(argv[1]));
  if (mydetector.detected)
  {
    mytracker.Start();
  }


}
