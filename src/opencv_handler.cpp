 /*
  * initialize opencv handler
  * do image processing
  * display results
  *
  * reference: http://answers.opencv.org/question/9511/how-to-find-the-intersection-point-of-two-lines/
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

  bool image_processing::intersection(Point2f o1, Point2f p1, Point2f o2, Point2f p2, Point2f &r)
  {
    Point2f x = o2 - o1;
    Point2f d1 = p1 - o1;
    Point2f d2 = p2 - o2;

    float cross = d1.x*d2.y - d1.y*d2.x;
    if (abs(cross) < /*EPS*/1e-8)
        return false;

    double t1 = (x.x * d2.y - x.y * d2.x)/cross;
    r = o1 + d1 * t1;
    return true;
  }

  void image_processing::findParallel(vector<int> &angles, vector<int> &indices)
  {
    /* return indices for lines are parallel*/
    vector<float> tmp;
    for(size_t i = 0; i < angles.size(); ++i)
    {
      auto it = std::find(tmp.begin(), tmp.end(), angles[i]);
      if ( it != tmp.end())
        tmp.push_back(angles[i]);
      else
      {
        indices.push_back(i);
        for(size_t j = 0; j < tmp.size(); ++j)
        {
          if (tmp[j] == angles[i])
            indices.push_back(j);
        }
      }
    }

  }

  void image_processing::lineAngles(vector<Point2f> points, vector<int> &angles)
  {
    /* calculate all the angles of detected lines*/
    float angle;
    for(size_t i = 0; i < points.size(); i+=2)
    {
      angle = atan2(points[i].y - points[i+1].y, points[i].x-points[i+1].x) * 180 /CV_PI;
      angles.push_back(angle);
    }
    for (size_t i = 0; i < angles.size(); i++)
      cout << angles[i] << endl;
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
      //image_processing::cornerFinder();
      //imshow("frame", image_processing::img);
      if (waitKey(30) == 'q') break;
    }

    return 0;
  }

  void image_processing::houghLineFinder()
  {
    Mat copy = image_processing::img;
    //cvtColor(copy, copy, CV_BGR2GRAY);
    //blur(copy, copy, Size(2,2));
    Canny(copy, image_processing::mask, 50, 300, 3);
    vector<Vec2f> lines_pos;
    vector<Point2f> drawing_pts;
    HoughLines(mask, lines_pos, 1, CV_PI / 180, 120, 50, 10);
    for (size_t i = 0; i < lines_pos.size(); ++i)
    {
      cout<< "found lines" << endl;
      float rho = lines_pos[i][0], theta = lines_pos[i][1];
      Point pt1, pt2;
      double a = cos(theta), b = sin(theta);
      double x0 = a*rho, y0 = b*rho;
      pt1.x = cvRound(x0 + 1000*(-b));
      pt1.y = cvRound(y0 + 1000*(a));
      pt2.x = cvRound(x0 - 1000*(-b));
      pt2.y = cvRound(y0 - 1000*(a));
      line( copy, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
      drawing_pts.push_back(pt1);
      drawing_pts.push_back(pt2);
    }

    /* find intersection points*/
    Point2f inter_pt;
    vector<Point2f> line_intersc;
    for (size_t i = 0; i < drawing_pts.size(); i+=4)
    {
      if (image_processing::intersection(drawing_pts[i], drawing_pts[i+2],
                        drawing_pts[i+1],drawing_pts[i+3], inter_pt))
        line_intersc.push_back(inter_pt);
    }

    /* find parallel lines*/
    vector<Point2f> line_parallel;
    vector<int> line_angles;
    vector<int> parallel_indices;
    image_processing::lineAngles(drawing_pts, line_angles);
    image_processing::findParallel(line_angles, parallel_indices);
    cout << "parallel indicies " << parallel_indices.size() << endl;
    for(size_t i = 0; i < parallel_indices.size(); ++i)
    {
      if ( line_parallel.size() < line_intersc.size())
        line_parallel.push_back(drawing_pts[parallel_indices[i]*2]);
      if ( line_parallel.size() < line_intersc.size())
        line_parallel.push_back(drawing_pts[parallel_indices[i]*2+1]);
    }

    //TODO:
    // make sure two vertices have the same length

    cout << "angle size = " << line_parallel.size() << " " << "intersection size = " << line_intersc.size() << endl;

    if( line_intersc.size() >= 4)
    {
      // find homograph based on the interest points
      Mat H = findHomography( line_intersc, line_parallel, CV_RANSAC );
      vector<Point2f> obj_corners(4);
      obj_corners[0] = cvPoint(0,0);
      obj_corners[1] = cvPoint(copy.cols, 0);
      obj_corners[2] = cvPoint(copy.cols, copy.rows);
      obj_corners[3] = cvPoint(0, copy.rows);
      vector<Point2f> scene_corners(4);

      perspectiveTransform(obj_corners, scene_corners, H);

      line( copy, scene_corners[0] + Point2f( copy.cols, 0), scene_corners[1] + Point2f( copy.cols, 0), Scalar(0, 255, 0), 4 );
      line( copy, scene_corners[1] + Point2f( copy.cols, 0), scene_corners[2] + Point2f( copy.cols, 0), Scalar( 0, 255, 0), 4 );
      line( copy, scene_corners[2] + Point2f( copy.cols, 0), scene_corners[3] + Point2f( copy.cols, 0), Scalar( 0, 255, 0), 4 );
      line( copy, scene_corners[3] + Point2f( copy.cols, 0), scene_corners[0] + Point2f( copy.cols, 0), Scalar( 0, 255, 0), 4 );

    }
    else
      cout << "Not enough feature points" << endl;
    image_processing::result = copy;

    namedWindow("result", WINDOW_AUTOSIZE);
    imshow("result", copy);
  }

  void image_processing::cornerFinder()
  {
    Mat copy = image_processing::img;
    Mat dst, dst_norm, dst_norm_scaled;
    dst = Mat::zeros( copy.size(), CV_32FC1);

    /// Detector parameters
    int blockSize = 5;
    int apertureSize = 3;
    double k = 0.04;

    cvtColor(copy, copy, CV_BGR2GRAY);
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
                 circle( image_processing::img, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
                }
            }
       }
    /// Showing the result
    //namedWindow( "corners_window", CV_WINDOW_AUTOSIZE );
    //imshow( "corners_window", dst_norm_scaled );
  }


} // namespace opencv_handler
