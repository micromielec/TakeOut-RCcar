#include <ros/ros.h>
//pcl 라이브러리를 사용하므로 헤더파일 추가
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include "std_msgs/Int32.h"
#include <pcl/filters/voxel_grid.h>


ros::Publisher pub;
ros::Publisher pub_distance;

void 
cloud_cb (const pcl::PCLPointCloud2ConstPtr& cloud)
{
  pcl::PCLPointCloud2 cloud_filtered; 

  // 복셀화
  pcl::VoxelGrid<pcl::PCLPointCloud2> sor;
  sor.setInputCloud (cloud); //laser_to_pointcloud_node.cpp 에서 LiDAR의 Laserscan값을 변환한 것이 cloud값, 이를 복셀화하는 코드에 input으로 설정합니다 
  sor.setLeafSize (0.1, 0.1, 0.1); //복셀 사이즈 지정 (너무 작은 값을 입력하면 샘플링 에러 메시지가 뜹니다)
  sor.filter (cloud_filtered); //복셀화된 데이터

  pcl::PCLPointCloud2 point_cloud2;
  pcl::PointCloud<pcl::PointXYZ> point_cloud;
  
  pcl::fromPCLPointCloud2( cloud_filtered, point_cloud);
  pcl::toPCLPointCloud2(point_cloud, point_cloud2);
  float min_distance=100000;
  
  for(unsigned int i=0;i<point_cloud.points.size();i++){
    if(point_cloud.points[i].x<min_distance)
      min_distance=point_cloud.points[i].x;
  }
  std::cout<<min_distance<<std::endl;
  if (min_distance!=100000){
    std_msgs::Int32 distance_msg;
    distance_msg.data=(min_distance*100);
    pub_distance.publish(distance_msg);
  }
  //복셀화된 데이터 퍼블리시
  pub.publish (cloud_filtered);
}

int
main (int argc, char** argv)
{
  ros::init (argc, argv, "my_pcl_tutorial");
  ros::NodeHandle nh;

  // cloud 라는 이름의 cloud_cb함수를 통해 나온 복셀화된 데이터를 서브스크라이브
  ros::Subscriber sub = nh.subscribe ("cloud", 1, cloud_cb);

  // output이라는 이름의 PointCloud2 데이터를 퍼블리시
  pub = nh.advertise<pcl::PCLPointCloud2> ("output", 1);
  pub_distance=nh.advertise<std_msgs::Int32>("/distance",1);

  // Spin
  ros::spin ();
}