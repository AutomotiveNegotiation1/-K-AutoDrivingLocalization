#ifndef ROS_TEST__VISIBILITY_CONTROL_H_
#define ROS_TEST__VISIBILITY_CONTROL_H_
#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define ROS_TEST_EXPORT __attribute__ ((dllexport))
    #define ROS_TEST_IMPORT __attribute__ ((dllimport))
  #else
    #define ROS_TEST_EXPORT __declspec(dllexport)
    #define ROS_TEST_IMPORT __declspec(dllimport)
  #endif
  #ifdef ROS_TEST_BUILDING_LIBRARY
    #define ROS_TEST_PUBLIC ROS_TEST_EXPORT
  #else
    #define ROS_TEST_PUBLIC ROS_TEST_IMPORT
  #endif
  #define ROS_TEST_PUBLIC_TYPE ROS_TEST_PUBLIC
  #define ROS_TEST_LOCAL
#else
  #define ROS_TEST_EXPORT __attribute__ ((visibility("default")))
  #define ROS_TEST_IMPORT
  #if __GNUC__ >= 4
    #define ROS_TEST_PUBLIC __attribute__ ((visibility("default")))
    #define ROS_TEST_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define ROS_TEST_PUBLIC
    #define ROS_TEST_LOCAL
  #endif
  #define ROS_TEST_PUBLIC_TYPE
#endif
#endif  // ROS_TEST__VISIBILITY_CONTROL_H_
// Generated 22-Sep-2022 11:50:59
// Copyright 2019-2020 The MathWorks, Inc.
