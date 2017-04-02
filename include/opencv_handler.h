#ifndef AR_OPENCV_HANDLER_H_
#define AR_OPENCV_HANDLER_H_

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/video/video.hpp>
#include <stdio.h>
#include <algorithm>

using namespace cv;
using namespace std;

namespace opencv_handler
{

  class image_processing
  {
    private:
      Mat img;
      Mat mask;
      Mat result;

      bool intersection(Point2f o1, Point2f p1, Point2f o2, Point2f p2, Point2f &r);
      void findParallel(vector<int> &angles, vector<int> &indices);
      void lineAngles(vector<Point2f> points, vector<int> &angles);

    public:
      image_processing();
      ~image_processing();

      int imgStreamIn( int arg);
      void houghLineFinder();
      void cornerFinder();

  };

} // namespace opencv_handler

#endif // AR_OPENCV_HANDLER_H_
