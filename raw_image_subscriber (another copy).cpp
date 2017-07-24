#include <ros/ros.h> //for subscriber and such
#include <image_transport/image_transport.h> //handles images
#include <ros/types.h> //strings
#include <cv_bridge/cv_bridge.h> //changes to CV image
#include <sensor_msgs/image_encodings.h> //helps ^
#include <opencv2/highgui/highgui.hpp> //CV image to JPG
#include <fstream> //writing csv
#include <iostream> //writing csv
#include <iomanip>
#include <std_msgs/Empty.h>
using namespace std;

void imageCallback(const sensor_msgs::ImageConstPtr& msg) //called whenever subscriber sees message
{
	ofstream csv;
	cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8); //convert to CV image
	ros::Time begin = ros::Time::now();
	string framename = "/mnt/ssd/frames/";
	ostringstream ostr;
	ostr << setfill('0') << setw(10) << begin.sec;
	ostr << "_";	
	ostr << setfill('0') << setw(10) << begin.nsec;
	framename = framename + ostr.str() + ".jpg";
	cv::imwrite(framename,cv_ptr->image); //write to image
	ROS_INFO("%i_%i",begin.sec,begin.nsec);
	csv.open("/mnt/ssd/csv/camera_frames.csv",ios::app); //append to CSV
	csv << framename + "\n";
	csv.close();
}

void tivaCallback(const std_msgs::Empty::ConstPtr& msg) {
	//
}

void arduinoCallback(const std_msgs::Empty::ConstPtr& msg) {

}

int main(int argc, char **argv)
{
	ros::Time::init(); //the time can be easily set to a certain value if needed, i.e. make every run start at 0
	ros::init(argc, argv, "image_listener"); //starts node
	ros::NodeHandle nh;
	image_transport::ImageTransport it(nh);
	image_transport::Subscriber sub = it.subscribe("/pg_12280229/image_raw",1,imageCallback); //sets up subscriber node
	ros::Publisher mcdatacall_pub = nh.advertise<std_msgs::Empty>("/mcdatacall",1000);
	ros::Subscriber arduinosub = nh.subscribe("/arduino",1,arduinoCallback);
	ros::Subscriber tivasub = nh.subscribe("/tiva",1,tivaCallback);
	ros::spin();
}
