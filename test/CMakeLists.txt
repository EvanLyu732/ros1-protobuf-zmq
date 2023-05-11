include(FetchContent)
message(STATUS "ready to fetch protobuf3.14")
FetchContent_Declare(
        protobuf3.14
        GIT_REPOSITORY https://github.com/protocolbuffers/protobuf
        GIT_TAG v3.14.0
)

FetchContent_MakeAvailable(protobuf3.14)

add_executable(camera_ros_sub_node
        test_camera_ros.cpp
        ${PROJECT_SOURCE_DIR}/ros_proto/gen/sensor_msgs/CompressImage.pb.cc
        ${PROJECT_SOURCE_DIR}/ros_proto/gen/std_msgs/Header.pb.cc
        ${PROJECT_SOURCE_DIR}/ros_proto/gen/std_msgs/Time.pb.cc
        )
set_target_properties(camera_ros_sub_node PROPERTIES
        IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/external/libprotobuf3.14"
        )
include_directories(${PROJECT_SOURCE_DIR})
include_directories(/usr/ros_proto/gen)

# link_directories(${PROJECT_SOURCE_DIR}/external/libprotobuf3.14)
# find_package(OpenCV REQUIRED)

# find_library(Protobuf_Lib
#         NAME protobuf
#         HINTS "${PROJECT_SOURCE_DIR}/external/libprotobuf3.14"
#         )
# #
# find_library(Protobuf_Lite_Lib
#         NAME protobuf-lite
#         HINTS "${PROJECT_SOURCE_DIR}/external/libprotobuf3.14"
#         )

target_link_libraries(camera_ros_sub_node PRIVATE
        ${Protobuf_Lib}
        ${Protobuf_Lite_lib}
        ${Protobuf_LIBRARIES}
        # ${OpenCv_LIBRARIES}
        pthread
        zmq
        )
