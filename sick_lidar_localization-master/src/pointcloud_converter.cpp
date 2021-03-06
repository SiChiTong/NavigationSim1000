/*
 * @brief pointcloud_converter subscribes to sim_loc_driver messages (type sick_lidar_localization::SickLocResultPortTelegramMsg),
 * converts them to PointCloud2 and publishes PointCloud2 messages on topic "/cloud".
 *
 * It also serves as an usage example for sick_lidar_localization and shows how to subscribe and use the
 * sick_lidar_localization messages.
 *
 * Copyright (C) 2019 Ing.-Buero Dr. Michael Lehning, Hildesheim
 * Copyright (C) 2019 SICK AG, Waldkirch
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of SICK AG nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission
 *     * Neither the name of Ing.-Buero Dr. Michael Lehning nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *      Authors:
 *         Michael Lehning <michael.lehning@lehning.de>
 *
 *  Copyright 2019 SICK AG
 *  Copyright 2019 Ing.-Buero Dr. Michael Lehning
 *
 */
#include <ros/ros.h>

#include "sick_lidar_localization/pointcloud_converter.h"

int main(int argc, char** argv)
{
  // Ros configuration and initialization
  ros::init(argc, argv, "pointcloud_converter");
  ros::NodeHandle nh;
  ROS_INFO_STREAM("pointcloud_converter started.");
  
  std::string result_telegrams_topic = "/sick_lidar_localization/driver/result_telegrams"; // default topic to publish result port telegram messages (type SickLocResultPortTelegramMsg)
  ros::param::param<std::string>("/sick_lidar_localization/driver/result_telegrams_topic", result_telegrams_topic, result_telegrams_topic);
  
  // Init verifier to compare and check sim_loc_driver and sim_loc_test_server messages
  sick_lidar_localization::PointCloudConverter pointcloud_converter(&nh);
  
  // Subscribe to sim_loc_driver messages
  ros::Subscriber result_telegram_subscriber = nh.subscribe(result_telegrams_topic, 1, &sick_lidar_localization::PointCloudConverter::messageCbResultPortTelegrams, &pointcloud_converter);
 
  // Start pointcloud converter thread
  pointcloud_converter.start();
  
  //////// Add by TungNV
  ROS_INFO("pointcloud_converter.cpp-83-Enable Sim1000");
  system("sleed 3");
  // rosservice call SickLocColaTelegram "{cola_ascii_request: 'sMN LocStartLocalizing', wait_response_timeout: 1}"
  system("rosservice call /SickLocColaTelegram {\"cola_ascii_request: 'sMN LocStartLocalizing', wait_response_timeout: 0\"}");
  ///////////////////////////////////////////////////////////////////////////////////////
  
  
  // Run ros event loop
  ros::spin();
  
  std::cout << "pointcloud_converter finished." << std::endl;
  ROS_INFO_STREAM("pointcloud_converter finished.");
  pointcloud_converter.stop();
  std::cout << "pointcloud_converter exits." << std::endl;
  ROS_INFO_STREAM("pointcloud_converter exits.");
  return 0;
}
