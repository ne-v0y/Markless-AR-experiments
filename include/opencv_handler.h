#ifndef AR_OPENCV_HANDLER_H_
#define AR_OPENCV_HANDLER_H_

#include <opencv2/opencv.hpp>
/*
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/video/video.hpp>
*/
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/video.hpp>
#include <stdio.h>

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

    public:
      image_processing();
      ~image_processing();

      int imgStreamIn( int arg);
      void houghLineFinder();
  };

} // namespace opencv_handler

#endif // AR_OPENCV_HANDLER_H_
