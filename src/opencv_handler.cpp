 /*
  * initialize opencv handler
  * do image processing
  * display results
  *
  */

#include "opencv_handler.h"

namespace opencv_handler
{

  image_processing::image_processing()
  {

  }

  image_processing::~image_processing()
  {

  }


  int image_processing::imgStreamIn(int arg)
  {
    VideoCapture cap(arg);
    if (!cap.isOpened())
      return -1;

    namedWindow("frame", WINDOW_AUTOSIZE);
    for (;;)
    {
      cap >> image_processing::img;
      image_processing::houghLineFinder();
      imshow("frame", image_processing::img);
      if (waitKey(30) == 'q') break;
    }

    return 0;
  }

  void image_processing::houghLineFinder()
  {
    Mat copy = image_processing::img;
    Canny(copy, image_processing::mask, 100, 200, 3);
    vector<Vec4i> lines_pos;
    HoughLinesP(mask, lines_pos, 1, CV_PI / 180, 50, 60, 5);
    for (size_t i = 0; i < lines_pos.size(); ++i)
    {
      Vec4i l = lines_pos[i];
      line(copy, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);
    }
    image_processing::result = copy;

    namedWindow("mask", WINDOW_AUTOSIZE);
    imshow("mask", image_processing::mask);
    namedWindow("result", WINDOW_AUTOSIZE);
    imshow("result", image_processing::result);
  }

} // namespace opencv_handler
