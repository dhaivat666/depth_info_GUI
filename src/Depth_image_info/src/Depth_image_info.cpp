#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sensor_msgs/PointCloud2.h>
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

using namespace cv;
using namespace std;

void disparityImageCallback(const sensor_msgs::ImageConstPtr& disparity)
{
    cout << "Inside callback!" << endl;
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
        cv_ptr = cv_bridge::toCvCopy(disparity, sensor_msgs::image_encodings::TYPE_32FC1);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

   for(int i=0; i < cv_ptr->image.rows; i++)
    {
    for(int j=0; j < cv_ptr->image.cols; j++)
        {
            printf("%f\n", cv_ptr->image.at<float>(i, j));
        }
    }

   //imshow("Depth image!", cv_ptr->image);
   waitKey(0);
}


int main (int argc, char **argv)
{
  ros::init(argc, argv, "Depth_image_info");
  namedWindow("Depth image!", WINDOW_AUTOSIZE );
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  ros::Subscriber sub = nh.subscribe("/depth_map/image", 1, disparityImageCallback);
  //pub = it.advertise("/DEM_IMAGE", 1);
  ros::spin ();
}
