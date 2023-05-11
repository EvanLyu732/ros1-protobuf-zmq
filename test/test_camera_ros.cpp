//
// Created by ubuntu on 23-4-26.
//

#include "sensor_msgs/CompressImage.pb.h"
#include <iostream>
#include <protobuf_zmq.h>

#include <thread>

using sensor_msgs::CompressImage;
using zmq_protobuf::ZmqFactory;

void FetchImg(const CompressImage img) { /* Show Img using OpenCV */
  /* For convenience usage, turn protomsg into rosmsg */

  /* Use CV bridge for display image */
}

int main() {
  /*
   * step1. produce a factory
   */
  auto facotry = zmq_protobuf::ZmqFactory<CompressImage>();
  std::cout << "step1. produce a factory" << std::endl;

  /*
   * step2. generate subscriber
   */
  auto listener = facotry.produceIPCSubscriber("cam_1");
  std::cout << "step2. generate subscriber for channel file: cam_1"
            << std::endl;

  /*
   * step3. bind callback
   */
  listener->bindCallBack(&FetchImg);
  std::cout << "step3. bind callback" << std::endl;

  /*
   * step4. blocking run
   */
  std::thread([&listener]() {
    std::cout << "step4. subscriber ready to run" << std::endl;
    listener->sync_run();
  }).join();
}
