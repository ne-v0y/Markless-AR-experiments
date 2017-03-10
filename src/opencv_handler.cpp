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
      image_processing::cornerFinder();
      imshow("frame", image_processing::img);
      if (waitKey(30) == 'q') break;
    }

    return 0;
  }

  void image_processing::houghLineFinder()
  {
    Mat copy = image_processing::img;
    blur(copy, copy, Size(4,4));
    Canny(copy, image_processing::mask, 100, 300, 3);
    vector<Vec2f> lines_pos;
    HoughLines(mask, lines_pos, 1, CV_PI / 180, 100, 0, 5);
    for (size_t i = 0; i < lines_pos.size(); ++i)
    {
      float rho = lines_pos[i][0], theta = lines_pos[i][1];
      Point pt1, pt2;
      double a = cos(theta), b = sin(theta);
      double x0 = a*rho, y0 = b*rho;
      pt1.x = cvRound(x0 + 1000*(-b));
      pt1.y = cvRound(y0 + 1000*(a));
      pt2.x = cvRound(x0 - 1000*(-b));
      pt2.y = cvRound(y0 - 1000*(a));
      line( copy, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
    }
    image_processing::result = copy;

    namedWindow("mask", WINDOW_AUTOSIZE);
    imshow("mask", image_processing::mask);
  }

  void image_processing::cornerFinder()
  {
    Mat copy = image_processing::img;
    Mat dst, dst_norm, dst_norm_scaled;
    dst = Mat::zeros( copy.size(), CV_32FC1);

    /// Detector parameters
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;

    /// Detecting corners
    cornerHarris( copy, dst, blockSize, apertureSize, k, BORDER_DEFAULT );

    /// Normalizing
    normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    convertScaleAbs( dst_norm, dst_norm_scaled );

    /// Drawing a circle around corners
    for( int j = 0; j < dst_norm.rows ; j++ )
       { for( int i = 0; i < dst_norm.cols; i++ )
            {
              if( (int) dst_norm.at<float>(j,i) > 200 )
                {
                 circle( dst_norm_scaled, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
                }
            }
       }
    /// Showing the result
    namedWindow( "corners_window", CV_WINDOW_AUTOSIZE );
    imshow( "corners_window", dst_norm_scaled );
  }

  bool image_processing::findPlane()
  {

  }

} // namespace opencv_handler
