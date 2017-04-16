#ifndef AR_DETECTIONS_H_
#define AR_DETECTIONS_H_

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/video/video.hpp>
#include <stdio.h>
#include <algorithm>
#include <string>

using namespace cv;
using namespace std;

namespace opencv_handler
{

  class Detections
  {
    private:
      Mat img;
      Mat mask;
      struct sorting {
        bool operator() (cv::Point pt1, cv::Point pt2) { return (pt1.y < pt2.y);}
      } pts_sorting;

      bool intersection(Point2f o1, Point2f p1, Point2f o2, Point2f p2, Point2f &r);
      void findParallel(vector<int> &angles, vector<int> &indices);
      void lineAngles(Mat &img, vector<Point2f> points, vector<int> &angles);

    public:
      Detections();
      ~Detections();

      Mat result;
      bool using_img; // if only testing with an image, then not triggering tracking
      bool detected;
      int success_count;
      Rect2d bounding_box;

      int imgStreamIn( int arg);
      void houghLineFinder();
      void houghLinePFinder();
      void findVanishing();
      void cornerFinder();
  };

} // namespace opencv_handler

#endif // AR_DETECTIONS_H_
