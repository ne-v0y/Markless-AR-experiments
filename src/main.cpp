 /*
  * main.cpp
  *
  * Created on: 20 Feb 2017
  * Author    : Noni Hua
  *
  * Brief     : Main function that runs markless augmented reality
  *
  */

#include <stdio.h>
#include <stdlib.h>
#include "opencv_handler.cpp"

using namespace std;
using namespace opencv_handler;

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    cout << "Usage: node <video input>" << endl;
    return -1;
  }

  image_processing proc;
  proc.imgStreamIn(atoi(argv[1]));
}
