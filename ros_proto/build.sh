#!/bin/bash
set -x

############################
# Test script, merly useless
############################

protoc -I. --cpp_out=./gen --experimental_allow_proto3_optional sensor_msgs/CompressImage.proto sensor_msgs/Image.proto sensor_msgs/PointCloud2.proto 
