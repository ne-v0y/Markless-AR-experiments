 /*
  * initialize opencv handler
  * do image processing
  * display results
  *
  *
  */

#include "Detections.h"

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
    if (arg == 3)
    {
      /* using image instead of video*/
      image_processing::img = imread("./img/0.jpg", CV_LOAD_IMAGE_COLOR);
      image_processing::houghLinePFinder();
      waitKey(0);                                          // Wait for a keystroke in the window
    }
    else
    {
      VideoCapture cap(arg);
      if (!cap.isOpened())
        return -1;

      // namedWindow("frame", WINDOW_AUTOSIZE);
      for (;;)
      {
        cap >> image_processing::img;
        image_processing::houghLinePFinder();
        //image_processing::cornerFinder();
        //imshow("frame", image_processing::img);
        if (waitKey(30) == 'q') break;
      }

    }

    return 0;
  }

  // void image_processing::houghLineFinder()
  // {
  //   Mat copy = image_processing::img;
  //   //cvtColor(copy, copy, CV_BGR2GRAY);
  //   //blur(copy, copy, Size(2,2));
  //   Canny(copy, image_processing::mask, 10, 300, 3);
  //   vector<Vec2f> lines_pos;
  //   vector<Point2f> drawing_pts;
  //   HoughLines(mask, lines_pos, 1, CV_PI / 180, 120, 100, 50);
  //   for (size_t i = 0; i < lines_pos.size(); ++i)
  //   {
  //     float rho = lines_pos[i][0], theta = lines_pos[i][1];
  //     Point pt1, pt2;
  //     double a = cos(theta), b = sin(theta);
  //     double x0 = a*rho, y0 = b*rho;
  //     pt1.x = cvRound(x0 + 1000*(-b));
  //     pt1.y = cvRound(y0 + 1000*(a));
  //     pt2.x = cvRound(x0 - 1000*(-b));
  //     pt2.y = cvRound(y0 - 1000*(a));
  //     line( copy, pt1, pt2, Scalar(0,0,255), 1, CV_AA);
  //     drawing_pts.push_back(pt1);
  //     drawing_pts.push_back(pt2);
  //   }
  //
  //   /* find intersection points*/
  //   Point2f inter_pt;
  //   vector<Point2f> line_intersc;
  //   for (size_t i = 0; i < drawing_pts.size(); i+=4)
  //   {
  //     if (image_processing::intersection(drawing_pts[i], drawing_pts[i+1],
  //                       drawing_pts[i+2],drawing_pts[i+3], inter_pt))
  //       line_intersc.push_back(inter_pt);
  //   }
  //
  //   /* find parallel lines*/
  //   vector<Point2f> line_parallel;
  //   vector<int> line_angles;
  //   vector<int> parallel_indices;
  //   image_processing::lineAngles(copy, drawing_pts, line_angles);
  //   image_processing::findParallel(line_angles, parallel_indices);
  //   cout << "parallel indicies " << parallel_indices.size() << endl;
  //   for(size_t i = 0; i < parallel_indices.size(); ++i)
  //   {
  //     if ( line_parallel.size() < line_intersc.size())
  //       line_parallel.push_back(drawing_pts[parallel_indices[i]*2]);
  //     if ( line_parallel.size() < line_intersc.size())
  //       line_parallel.push_back(drawing_pts[parallel_indices[i]*2+1]);
  //   }
  //
  //   cout << "angle size = " << line_parallel.size() << " " << "intersection size = " << line_intersc.size() << endl;
  //
  //   if( line_intersc.size() >= 4 and line_parallel.size() >= 4)
  //   {
  //     /* find homograph based on the interest points*/
  //     Mat H = findHomography( line_intersc, line_parallel, CV_RANSAC );
  //     vector<Point2f> obj_corners(4);
  //     obj_corners[0] = cvPoint(0,0);
  //     obj_corners[1] = cvPoint(copy.cols, 0);
  //     obj_corners[2] = cvPoint(copy.cols, copy.rows);
  //     obj_corners[3] = cvPoint(0, copy.rows);
  //     vector<Point2f> scene_corners(4);
  //     perspectiveTransform(obj_corners, scene_corners, H);
  //
  //     /* find the rvec and tevc*/
  //     Mat cameraMatrix(3,3,DataType<double>::type);
  //     setIdentity(cameraMatrix);
  //     cout << "Initial cameraMatrix: " << cameraMatrix << endl;
  //
  //     Mat distCoeffs(4,1,DataType<double>::type);
  //     distCoeffs.at<double>(0) = 0;
  //     distCoeffs.at<double>(1) = 0;
  //     distCoeffs.at<double>(2) = 0;
  //     distCoeffs.at<double>(3) = 0;
  //
  //     Mat rvec(3,1, DataType<double>::type);
  //     Mat tvec(3,1, DataType<double>::type);
  //
  //
  //     vector<Point3f> pts3d;
  //     Point3f tmp;
  //     int rows = copy.rows;
  //     int cols = copy.cols;
  //     tmp.x = 0;
  //     tmp.y = 0;
  //     tmp.z = 0;
  //     pts3d.push_back(tmp);
  //     tmp.x = 0;
  //     tmp.y = cols - 1;
  //     tmp.z = 0;
  //     pts3d.push_back(tmp);
  //     tmp.x = rows - 1;
  //     tmp.y = cols - 1;
  //     tmp.z = 0;
  //     pts3d.push_back(tmp);
  //     tmp.x = rows - 1;
  //     tmp.y = 0;
  //     tmp.z = 0;
  //     pts3d.push_back(tmp);
  //     solvePnPRansac(pts3d, obj_corners, cameraMatrix, distCoeffs, rvec, tvec);
  //     cout << "rvec = " << rvec << endl;
  //     cout << "tvec = " << tvec << endl;
  //
  //     line( copy, scene_corners[0] + Point2f( copy.cols, 0), scene_corners[1] + Point2f( copy.cols, 0), Scalar(0, 255, 0), 4 );
  //     line( copy, scene_corners[1] + Point2f( copy.cols, 0), scene_corners[2] + Point2f( copy.cols, 0), Scalar( 0, 255, 0), 4 );
  //     line( copy, scene_corners[2] + Point2f( copy.cols, 0), scene_corners[3] + Point2f( copy.cols, 0), Scalar( 0, 255, 0), 4 );
  //     line( copy, scene_corners[3] + Point2f( copy.cols, 0), scene_corners[0] + Point2f( copy.cols, 0), Scalar( 0, 255, 0), 4 );
  //
  //   }
  //   else
  //     cout << "Not enough feature points" << endl;
  //   image_processing::result = copy;
  //
  //   namedWindow("result", WINDOW_AUTOSIZE);
  //   imshow("result", copy);
  //   imshow("mask", mask);
  // }



  /* hough line P finder*/
  void image_processing::houghLinePFinder()
  {
    Mat copy = image_processing::img;
    //cvtColor(copy, copy, CV_BGR2GRAY);
    //blur(copy, copy, Size(2,2));
    Canny(copy, image_processing::mask, 10, 300, 3);
    vector<Vec4i> lines_pos;
    vector<Point2f> drawing_pts;
    HoughLinesP(mask, lines_pos, 1, CV_PI / 180, 60, 30, 10 );
    //cout << "Length of the points = " << lines_pos.size() << endl;
    Mat blank(480, 640, CV_8UC1, Scalar(0,0,0));
    for (size_t i = 0; i < lines_pos.size(); ++i)
    {
      //cout<< "found lines " << lines_pos[i][0] << " "<< lines_pos[i][1] << " "<< lines_pos[i][2] << " "<< lines_pos[i][3] <<endl;
      Point2f pt1, pt2;
      pt1.x = lines_pos[i][0]; pt1.y = lines_pos[i][1];
      pt2.x = lines_pos[i][2]; pt2.y = lines_pos[i][3];
      line( blank, pt1, pt2, Scalar(255,255,255), 1, CV_AA);
      drawing_pts.push_back(pt1);
      drawing_pts.push_back(pt2);
    }

    mask = blank + mask;
    /* find intersection points*/
    Point2f inter_pt;
    vector<Point2f> line_intersc;
    for (size_t i = 0; i < drawing_pts.size(); i+=2)
    {
      for (size_t j = i + 2; j < drawing_pts.size(); j+=2)
      {
        if (image_processing::intersection(drawing_pts[i], drawing_pts[i+1], drawing_pts[j],drawing_pts[j+1], inter_pt))
        {
          if ((inter_pt.x < 640) && (inter_pt.x > 0) && (inter_pt.y < 480) && (inter_pt.y > 0))
          {
            line_intersc.push_back(inter_pt);
          }
        }
      }
    }


    for (size_t t = 0; t < line_intersc.size(); t ++)
      circle(copy, line_intersc[t], 4, Scalar(0,0,255), -1, 8, 0);


    //RotatedRect bounding = minAreaRect(line_intersc);
    // TODO : only when lines intersections are larger than XX, check XX

    vector<Point2f> vertices, useful;
    if (! line_intersc.size() < 4)
      cout << "Not enough line intersections detected" << endl;
    else
    {
      Rect bounding = boundingRect(line_intersc);

      vertices.push_back(Point2f(bounding.x, bounding.y));
      vertices.push_back(Point2f(bounding.x + bounding.width, bounding.y));
      vertices.push_back(Point2f(bounding.x + bounding.width, bounding.y + bounding.height));
      vertices.push_back(Point2f(bounding.x, bounding.y+bounding.height));
      //Point2f tmp[4];
      //bounding.points(tmp);
      float diff_ = 9999;
      Point2f d;
      for (int i = 0; i < 4; i++)
      {
        line(copy, vertices[i], vertices[(i+1)%4], Scalar(0,100,0),2);
        d = vertices[i] - vertices[(i+1)%4];
        if (sqrt(d.x*d.x + d.y*d.y) < diff_)
          diff_ = sqrt(d.x*d.x + d.y*d.y);
      }
      cout << diff_ << endl;
      //cout << "bounding rectangel" << bounding << endl;

      // TODO: use some good algorithm to find most useful four points
      sort(line_intersc.begin(),  line_intersc.end(), image_processing::pts_sorting);
      for(size_t t =0; t < line_intersc.size(); t ++)
      {
        if (useful.empty() )
        {
          d = line_intersc[t] - line_intersc[(t+1)%line_intersc.size()];
          if ((useful.size() < 4 ) and (sqrt(d.x*d.x+d.y*d.y) > (diff_/3*2)))
          {
              useful.push_back(line_intersc[t]);
              circle(copy, line_intersc[t], 8, Scalar(0,0,0), -1,8,0);
              useful.push_back(line_intersc[t+1]);
              circle(copy, line_intersc[t+1], 8, Scalar(0,0,0), -1,8,0);

          }
        }
        else
        {
          d = line_intersc[t] - useful[useful.size()-1];
          if ((useful.size() < 4 ) and (sqrt(d.x*d.x+d.y*d.y) > (diff_/3*2)))
          {
              useful.push_back(line_intersc[t]);
              circle(copy, line_intersc[t], 8, Scalar(0,0,0), -1,8,0);
              //useful.push_back(line_intersc[t+1]);
              //circle(copy, line_intersc[t+1], 8, Scalar(0,0,0), -1,8,0);


          } /* end of internel if*/
        } /* end of useful.empty conditional*/


      } /* end of for loop*/
    }



    cout << "original = " << vertices.size() << " target = " << useful.size() << " " << endl;
    //rectangle(copy, bounding, Scalar(200,200,0), 2, 8, 0 );
    if( vertices.size() >= 4 and useful.size() >= 4)
    {
      /* find homograph based on the interest points*/
      Mat H = findHomography( vertices, useful, CV_RANSAC, 5);
      vector<Point2f> obj_corners(4);
      obj_corners[0] = cvPoint(0,0);
      obj_corners[1] = cvPoint(0, copy.rows-1);
      obj_corners[2] = cvPoint(copy.cols-1, copy.rows-1);
      obj_corners[3] = cvPoint(copy.cols-1, 0);
      vector<Point2f> scene_corners(4);
      perspectiveTransform(obj_corners, scene_corners, H);
      cout << "scene_corners " << scene_corners << endl;
      for (size_t i = 0; i < scene_corners.size(); i ++)
      {
        circle(copy, scene_corners[i], 4, Scalar(0, 255 ,0), -1, 8, 0);
        putText(copy, to_string(i), scene_corners[i], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,0,255), 2, 3,false );

      }

      //polylines(copy, scene_corners, true, Scalar(0,255,0), 1, 8, 0 );
      line( copy, scene_corners[0], scene_corners[2], Scalar(0, 255, 0), 4 );
      line( copy, scene_corners[2], scene_corners[1], Scalar( 0, 255, 0), 4 );
      line( copy, scene_corners[1], scene_corners[3], Scalar( 0, 255, 0), 4 );
      line( copy, scene_corners[3], scene_corners[0], Scalar( 0, 255, 0), 4 );


      /* find the rvec and tevc*/
      Mat cameraMatrix(3,3,DataType<double>::type);
      setIdentity(cameraMatrix);
      cout << "Initial cameraMatrix: " << cameraMatrix << endl;

      Mat distCoeffs(4,1,DataType<double>::type);
      distCoeffs.at<double>(0) = 0;
      distCoeffs.at<double>(1) = 0;
      distCoeffs.at<double>(2) = 0;
      distCoeffs.at<double>(3) = 0;

      Mat rvec(3,1, DataType<double>::type);
      Mat tvec(3,1, DataType<double>::type);


      vector<Point3f> pts3d;
      Point3f tmp;
      int rows = copy.rows;
      int cols = copy.cols;
      tmp.x = 0;
      tmp.y = 0;
      tmp.z = 0;
      pts3d.push_back(tmp);
      tmp.x = 0;
      tmp.y = cols - 1;
      tmp.z = 0;
      pts3d.push_back(tmp);
      tmp.x = rows - 1;
      tmp.y = cols - 1;
      tmp.z = 0;
      pts3d.push_back(tmp);
      tmp.x = rows - 1;
      tmp.y = 0;
      tmp.z = 0;
      pts3d.push_back(tmp);
      solvePnPRansac(pts3d, scene_corners, cameraMatrix, distCoeffs, rvec, tvec);
      cout << "rvec = " << rvec << endl;
      cout << "tvec = " << tvec << endl;
    }
    else
      cout << "Not enough feature points" << endl;
    image_processing::result = copy;

    namedWindow("result", WINDOW_AUTOSIZE);
    imshow("result", result);
    //imshow("mask", mask);
    imshow("blank", blank);
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


  /* -------------------------------------------------------------------------*/
  /* some helper functions, references: https://github.com/opencv/opencv/blob/master/modules/imgproc/src/min_enclosing_triangle.cpp#L1380-L1424*/
  bool image_processing::intersection(Point2f o1, Point2f p1, Point2f o2, Point2f p2, Point2f &r)
  {
    Point2f x = o2 - o1;
    Point2f d1 = p1 - o1;
    Point2f d2 = p2 - o2;

    float cross = d1.x*d2.y - d1.y*d2.x;
    //cout << cross << endl;
    if (abs(cross) < /*EPS*/200) // TODO: find the actual threshold
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

  void image_processing::lineAngles(Mat &img, vector<Point2f> points, vector<int> &angles)
  {
    /* calculate all the angles of detected lines*/
    int angle;
    for(size_t i = 0; i < points.size(); i+=2)
    {
      angle = atan2(points[i].y - points[i+1].y, points[i].x-points[i+1].x) * 180 /CV_PI;
      //if (angle == 180 or angle == 90 or angle == -180 or angle == -90)
      angles.push_back(angle);
      putText(img, to_string(angle), points[i], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255,0,255), 2, 3,false );
    }
  }


/* --------------------End of Detection --------------------------------------*/


} // namespace opencv_handler
