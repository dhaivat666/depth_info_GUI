#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sensor_msgs/PointCloud2.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <std_msgs/String.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/Float32MultiArray.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/PointCloud2.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <stereo_msgs/DisparityImage.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <stdlib.h>

using namespace cv;
using namespace std;
using namespace sensor_msgs;
using namespace message_filters;

Mat depth_image, left_rect_image;

void onMouse(int event, int x, int y, int flags, void* param)
{
    char text[100];
    Mat img2, img3;

    img2 = depth_image.clone();

    if (event == CV_EVENT_LBUTTONDOWN)
    {
	//  Vec3b p = img2.at<Vec3b>(y,x);
	cout << "Depth = " << img2.at<float>(y, x) << endl;
	//  sprintf(text, "Depth = %f", img2.at<float>(y, x));
    }
    /*else if (event == CV_EVENT_RBUTTONDOWN)
    {
        cvtColor(img, img3, CV_BGR2HSV);
        Vec3b p = img3.at<Vec3b>(y,x);
        sprintf(text, "H=%d, S=%d, V=%d", p[0], p[1], p[2]);
    }
    else
        sprintf(text, "x=%d, y=%d", x, y);*/

	//    putText(left_rect_image, text, Point(5,15), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0));
	    imshow("Left_rect image!",left_rect_image);	
}

void Depth_left_sync(const sensor_msgs::ImageConstPtr& depth, const sensor_msgs::ImageConstPtr& left_rect){
	    cout << "Reached Depth_left_sync!" << endl;
	    cv_bridge::CvImagePtr depth_ptr;
	    cv_bridge::CvImagePtr left_rect_ptr;
	    depth_ptr = cv_bridge::toCvCopy(depth, sensor_msgs::image_encodings::TYPE_32FC1);
            left_rect_ptr = cv_bridge::toCvCopy(left_rect, sensor_msgs::image_encodings::BGR8);
	    depth_image = depth_ptr->image;
	    left_rect_image = left_rect_ptr->image;
	    imshow("Left_rect image!",left_rect_image);	
//   	    setMouseCallback("Left_rect image!", onMouse, 0);
	    setMouseCallback("Left_rect image!", onMouse, 0);
	    waitKey(0);	
	    usleep(500000);
	
}


int main (int argc, char **argv)
{
  ros::init(argc, argv, "Depth_image_info");
  namedWindow("Left_rect image!", WINDOW_AUTOSIZE );
  ros::NodeHandle nh;
//  image_transport::ImageTransport it(nh);
  cout << "Reached main!" << endl;
  message_filters::Subscriber<Image> image_depth(nh, "/depth_map/image", 1);
  message_filters::Subscriber<Image> image_left_rect(nh, "/left_rgb_rect/image_rect_color", 1);
  TimeSynchronizer<Image, Image> sync(image_depth, image_left_rect, 10);
  sync.registerCallback(boost::bind(&Depth_left_sync, _1, _2));
  //setMouseCallback("Left_rect image!", onMouse, 0);
//  ros::Subscriber sub = nh.subscribe("/depth_map/image", 1, disparityImageCallback);
  //pub = it.advertise("/DEM_IMAGE", 1);
  
  ros::spin ();
  
}
