#ifndef AR_TRACKING_H_
#define AR_TRACKING_H_

#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>
#include <string>

using namespace cv;
using namespace std;

namespace opencv_handler
{
  class Tracking
  {

  public:
    Tracking();
    ~Tracking();

    Mat frame;
    Rect2d ROI;
    String method;
    int Start();
  };
} // namespace opencv_handler

#endif // AR_OPENCV_HANDLER_H_
