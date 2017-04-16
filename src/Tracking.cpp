/*
 * Tracking node
 * Using KLD tracker to track useful detection points
 *
 * Author: Noni YiFan Hua
 * Date: April 12th 2017
 */

#include "Tracking.h"

namespace opencv_handler
{

  Tracking::Tracking(){}

  Tracking::~Tracking(){}

  int Tracking::Start()
  {
    // TODO: add other tracking methods
    Tracking::method = "TLD";
    Ptr<Tracker> tracker = Tracker::create("TLD");
    if (tracker == NULL)
    {
      cout << "Error: can't initialize tracker." << endl;
      return -1;
    }

    tracker->init(Tracking::frame, Tracking::ROI);


    return 0;
  }
}
