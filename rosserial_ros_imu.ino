#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <ros.h>
#include <ros/time.h>
#include <geometry_msgs/TransformStamped.h>

//Set up adafruit 
Adafruit_BNO055 bno = Adafruit_BNO055(55);

//Set up the ros node and publisher
ros::NodeHandle nh;
geometry_msgs::TransformStamped pose_msg;
ros::Publisher pub_pose("adafruit/imu", &pose_msg);

void setup()
{

  nh.initNode();
  nh.advertise(pub_pose);
 
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    //Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");Serial.println("");
    while(1);
  } 
  bno.setExtCrystalUse(true);  
  delay(1000);  
}

void loop()
{ 
  // Get linear acceleration data from seneor
  imu::Vector<3> linearaccel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
   
  // Get quatrnion data from sensor 
  imu::Quaternion quat = bno.getQuat();

  //position data   
  pose_msg.transform.translation.x = linearaccel.x();
  pose_msg.transform.translation.y = linearaccel.y();
  pose_msg.transform.translation.z = linearaccel.z();
  // orientation data
  pose_msg.transform.rotation.x = quat.x();
  pose_msg.transform.rotation.y = quat.y();
  pose_msg.transform.rotation.z = quat.z(); 
  pose_msg.transform.rotation.w = quat.w();
  
  //pose_msg.header.stamp = nh.now();
  pub_pose.publish(&pose_msg);

  nh.spinOnce();
  delay(1);
}
