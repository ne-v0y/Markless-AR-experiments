 /*
  * initialize opencv handler
  * do image processing
  * display results
  *
  * Author: Noni YiFan Hua
  * Date: Feb 23 2017
  */

#include "Detections.h"

namespace opencv_handler
{

  Detections::Detections()
  {
    Detections::tracker = Tracker::create("TLD");
    if (Detections::tracker == NULL)
    {
      cout << "Error: can't initialize tracker." << endl;
    }

  }

  Detections::~Detections()
  {

  }

  int Detections::imgStreamIn(int arg)
  {
    if (arg == 3)
    {
      /* using image instead of video*/
      Detections::using_img = true;
      Detections::img = imread("./img/1.jpg", CV_LOAD_IMAGE_COLOR);
      Detections::houghLinePFinder();
      waitKey(0);                                          // Wait for a keystroke in the window
    }
    else
    {
      Detections::using_img = false;
      VideoCapture cap(arg);
      if (!cap.isOpened())
        return -1;

      // namedWindow("frame", WINDOW_AUTOSIZE);
      for (;;)
      {
        cap >> Detections::img;
        Detections::houghLinePFinder();
        //Detections::cornerFinder();
        //imshow("frame", Detections::img);
        if (waitKey(30) == 'q') break;
      }

    }

    return 0;
  }

  /* hough line P finder*/
  void Detections::houghLinePFinder()
  {
    Mat copy = Detections::img;
    //cvtColor(copy, copy, CV_BGR2GRAY);
    blur(copy, copy, Size(4,4));
    Canny(copy, Detections::mask, 10, 300, 3);
    vector<Vec4i> lines_pos;
    vector<Point2f> drawing_pts;
    HoughLinesP(mask, lines_pos, 1, CV_PI / 180, 60, 30, 10 );
    //cout << "Length of the points = " << lines_pos.size() << endl;
    Mat blank(480, 640, CV_8UC1, Scalar(0,0,0));
    Point2f d;
    for (size_t i = 0; i < lines_pos.size(); ++i)
    {
      //cout<< "found lines " << lines_pos[i][0] << " "<< lines_pos[i][1] << " "<< lines_pos[i][2] << " "<< lines_pos[i][3] <<endl;
      Point2f pt1, pt2;
      pt1.x = lines_pos[i][0]; pt1.y = lines_pos[i][1];
      pt2.x = lines_pos[i][2]; pt2.y = lines_pos[i][3];
      line( blank, pt1, pt2, Scalar(255,255,255), 1, CV_AA);
      d = pt1 - pt2;
      if (sqrt(d.x*d.x + d.y*d.y) > 100)
      {
        drawing_pts.push_back(pt1);
        drawing_pts.push_back(pt2);
      }
    }

    //mask = blank + mask;
    /* find intersection points*/
    Point2f inter_pt;
    vector<Point2f> line_intersc;
    for (size_t i = 0; i < drawing_pts.size(); i+=2)
    {
      for (size_t j = i + 2; j < drawing_pts.size(); j+=2)
      {
        if (Detections::intersection(drawing_pts[i], drawing_pts[i+1], drawing_pts[j],drawing_pts[j+1], inter_pt))
        {
          if ((inter_pt.x < 640) && (inter_pt.x > 0) && (inter_pt.y < 480) && (inter_pt.y > 0))
            line_intersc.push_back(inter_pt);
        }
      }
    }


    for (size_t t = 0; t < line_intersc.size(); t ++)
      circle(copy, line_intersc[t], 4, Scalar(0,0,255), -1, 8, 0);

    //RotatedRect bounding = minAreaRect(line_intersc);
    vector<Point2f> vertices, useful;
    if ( line_intersc.size() < 4)
      cout << "Not enough line intersections detected" << endl;
    else
    {
      Rect2d bounding = boundingRect(line_intersc);

      /* passing the bounding box to ROI*/
      /* TODO: add a good filtering and recovery scheme*/
      Detections::bounding_box = bounding;

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
      sort(line_intersc.begin(),  line_intersc.end(), Detections::pts_sorting);
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
      Detections::detected = true;

      if (Detections::detected)
      {
        Detections::tracking();
      }
      else
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
        // using calibrated parameters on my webcam
        // [588.5540560194831, 0, 311.4898582826918, 0, 588.3201007355697, 249.379129698565, 0, 0, 1]
        cameraMatrix.at<double>(0,0) = 588.5540560194831;
        cameraMatrix.at<double>(0,2) = 311.4898582826918;
        cameraMatrix.at<double>(1,1) = 588.3201007355697;
        cameraMatrix.at<double>(1,2) = 249.379129698565;
        cout << "Initial cameraMatrix: " << cameraMatrix << endl;

        Mat distCoeffs(5,1,DataType<double>::type);
        // using calibrated params: [-0.06095721652204921, -0.09321965925237759, 0.01404094207269403, 0.003574047262481977, 0]
        distCoeffs.at<double>(0) = -0.06095721652204921;
        distCoeffs.at<double>(1) = -0.09321965925237759;
        distCoeffs.at<double>(2) = 0.01404094207269403;
        distCoeffs.at<double>(3) = 0.003574047262481977;
        distCoeffs.at<double>(4) = 0;

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
        //solvePnPRansac(pts3d, scene_corners, cameraMatrix, distCoeffs, rvec, tvec);
        solvePnP(pts3d, useful, cameraMatrix, distCoeffs, rvec, tvec);
        cout << "rvec = " << rvec << endl;
        cout << "tvec = " << tvec << endl;
      } /* end of tracking condition*/


    }
    else
      cout << "Not enough useful feature points" << endl;
    Detections::result = copy;

    namedWindow("result", WINDOW_AUTOSIZE);
    imshow("result", result);
    //imshow("mask", mask);
    imshow("blank", blank);
  }

  int Detections::tracking()
  {
    cout << "Tracking" << endl;
    Detections::tracker->init(Detections::result, Detections::bounding_box);

    Detections::tracker->update(Detections::result,Detections::bounding_box);
    rectangle(Detections::result,Detections::bounding_box, Scalar( 255, 0, 0 ), 2, 1 );
    //imshow("tracker",Tracking::frame);


    return 0;
  }

  void Detections::cornerFinder()
  {
    Mat copy = Detections::img;
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
                 circle( Detections::img, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
                }
            }
       }
    /// Showing the result
    namedWindow( "corners_window", CV_WINDOW_AUTOSIZE );
    imshow( "corners_window", dst_norm_scaled );
  }


  /* -------------------------------------------------------------------------*/
  /* some helper functions, references: https://github.com/opencv/opencv/blob/master/modules/imgproc/src/min_enclosing_triangle.cpp#L1380-L1424*/
  bool Detections::intersection(Point2f o1, Point2f p1, Point2f o2, Point2f p2, Point2f &r)
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

  void Detections::findParallel(vector<int> &angles, vector<int> &indices)
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

  void Detections::lineAngles(Mat &img, vector<Point2f> points, vector<int> &angles)
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
