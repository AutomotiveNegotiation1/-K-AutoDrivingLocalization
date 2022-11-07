#ifndef DECAWAVE_UWB__VISIBILITY_CONTROL_H_
#define DECAWAVE_UWB__VISIBILITY_CONTROL_H_
#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define DECAWAVE_UWB_EXPORT __attribute__ ((dllexport))
    #define DECAWAVE_UWB_IMPORT __attribute__ ((dllimport))
  #else
    #define DECAWAVE_UWB_EXPORT __declspec(dllexport)
    #define DECAWAVE_UWB_IMPORT __declspec(dllimport)
  #endif
  #ifdef DECAWAVE_UWB_BUILDING_LIBRARY
    #define DECAWAVE_UWB_PUBLIC DECAWAVE_UWB_EXPORT
  #else
    #define DECAWAVE_UWB_PUBLIC DECAWAVE_UWB_IMPORT
  #endif
  #define DECAWAVE_UWB_PUBLIC_TYPE DECAWAVE_UWB_PUBLIC
  #define DECAWAVE_UWB_LOCAL
#else
  #define DECAWAVE_UWB_EXPORT __attribute__ ((visibility("default")))
  #define DECAWAVE_UWB_IMPORT
  #if __GNUC__ >= 4
    #define DECAWAVE_UWB_PUBLIC __attribute__ ((visibility("default")))
    #define DECAWAVE_UWB_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define DECAWAVE_UWB_PUBLIC
    #define DECAWAVE_UWB_LOCAL
  #endif
  #define DECAWAVE_UWB_PUBLIC_TYPE
#endif
#endif  // DECAWAVE_UWB__VISIBILITY_CONTROL_H_
// Generated 16-Sep-2022 15:58:40
// Copyright 2019-2020 The MathWorks, Inc.
