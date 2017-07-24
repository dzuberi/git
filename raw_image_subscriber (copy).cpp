#include <ros/ros.h> //for subscriber and such
#include <image_transport/image_transport.h> //handles images
#include <ros/types.h> //strings
#include <cv_bridge/cv_bridge.h> //changes to CV image
#include <sensor_msgs/image_encodings.h> //helps ^
#include <opencv2/highgui/highgui.hpp> //CV image to JPG
#include <fstream> //writing csv
#include <iostream> //writing csv
using namespace std;

void imageCallback(const sensor_msgs::ImageConstPtr& msg) //called whenever subscriber sees message
{
	ofstream csv;
	cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8); //convert to CV image
	ros::Time begin = ros::Time::now();	
	string framename = "/home/nvidia/frames/" + to_string(begin.sec)+ "_" + to_string(begin.nsec) + ".jpg"; //filename
	cv::imwrite(framename,cv_ptr->image); //write to image
	ROS_INFO("%i_%i",begin.sec,begin.nsec);
	csv.open("/home/nvidia/camera_frames.csv",ios::app); //append to CSV
	csv << framename + "\n";
	csv.close();
}

int main(int argc, char **argv)
{
	ros::Time::init(); //the time can be easily set to a certain value if needed, i.e. make every run start at 0
	ros::init(argc, argv, "image_listener"); //starts node
	ros::NodeHandle nh;
	image_transport::ImageTransport it(nh);
	image_transport::Subscriber sub = it.subscribe("/pg_12280229/image_raw",1,imageCallback); //sets up subscriber node
	ros::spin();
}
