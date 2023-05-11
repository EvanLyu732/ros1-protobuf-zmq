set(PROTOC_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/bin/v3.14/protoc)

macro(checkProtocVersion)
    set(CUR_WORKING_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    execute_process(
            COMMAND ${PROTOC_EXECUTABLE} --version
            OUTPUT_VARIABLE PROTOC_VERSION_STR
    )
    string(FIND ${PROTOC_VERSION_STR} 3.14.0 PROTOC_VERSION)
    if (${PROTOC_VERSION} STREQUAL 10)
        message(STATUS "valid protoc 3.14")
    else()
        message(FATAL_ERROR "Protoc version error")
    endif()
endmacro()


function(generateProtoFile MSG_FOLDER)
     foreach(MSG_TYPE ${MSG_FOLDER})
         set(MSG_CPP_OUT "${CMAKE_CURRENT_SOURCE_DIR}/gen")
         set(MSG_PROTO_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${MSG_TYPE}")
         file(GLOB PROTOS_SRC ${MSG_PROTO_PATH}/*.proto)
         foreach(PROTOS ${PROTOS_SRC})
             get_filename_component(PROTO_TEMP ${PROTOS} NAME BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
             set(PROTO_REALTIVE "${MSG_TYPE}/${PROTO_TEMP}")
             list(APPEND PROTO_SRCS ${PROTO_REALTIVE})
         endforeach()
         execute_process(
                 COMMAND ${PROTOC_EXECUTABLE} -I${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=${MSG_CPP_OUT} --experimental_allow_proto3_optional ${PROTO_SRCS}
         )
         #
         #  There are some bugs in below code, Can't fix it.
         #

        ##         # mkdir xxx_hdrs
        #         set(HDR_DIR "${MSG_CPP_OUT}/${MSG_TYPE}_hdrs")
        #         set(SRC_DIR "${MSG_CPP_OUT}/${MSG_TYPE}_srcs")
        #
        #         file(MAKE_DIRECTORY ${HDR_DIR})
        #         file(MAKE_DIRECTORY ${SRC_DIR})
        #
        #         file(GLOB PROTO_HDRS "${MSG_CPP_OUT}/${MSG_TYPE}/*.pb.h")
        #
        ##         file(COPY ${PROTO_HDRS} DESTINATION ${HDR_DIR})
        ##         file(COPY ${PROTO_SRCS} DESTINATION ${SRC_DIR})
        ##
        ##         set(TARGET_FOLDER ${MSG_CPP_OUT}/${MSG_TYPE})
        #         execute_process(
        #                 COMMAND cp -r ${MSG_CPP_OUT}/${MSG_TYPE}/*.pb.cc ${SRC_DIR}
        #                 COMMAND rm -rf ${TARGET_FOLDER}
        #                 COMMAND_ECHO STDOUT
        #                 RESULT_VARIABLE res
        #         )
     endforeach()
endfunction()

macro(InstallFiles MSG_FOLDER)
    set(MSG_CPP_OUT "${CMAKE_CURRENT_SOURCE_DIR}/gen")
    INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/ros_proto
            DESTINATION /usr
    )
endmacro()

list(APPEND MSG_FOLDER "sensor_msgs" "std_msgs")

checkProtocVersion()
generateProtoFile("${MSG_FOLDER}")
InstallFiles("${MSG_FOLDER}")


# Install
