#ifndef UBLOX_F9R__VISIBILITY_CONTROL_H_
#define UBLOX_F9R__VISIBILITY_CONTROL_H_
#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define UBLOX_F9R_EXPORT __attribute__ ((dllexport))
    #define UBLOX_F9R_IMPORT __attribute__ ((dllimport))
  #else
    #define UBLOX_F9R_EXPORT __declspec(dllexport)
    #define UBLOX_F9R_IMPORT __declspec(dllimport)
  #endif
  #ifdef UBLOX_F9R_BUILDING_LIBRARY
    #define UBLOX_F9R_PUBLIC UBLOX_F9R_EXPORT
  #else
    #define UBLOX_F9R_PUBLIC UBLOX_F9R_IMPORT
  #endif
  #define UBLOX_F9R_PUBLIC_TYPE UBLOX_F9R_PUBLIC
  #define UBLOX_F9R_LOCAL
#else
  #define UBLOX_F9R_EXPORT __attribute__ ((visibility("default")))
  #define UBLOX_F9R_IMPORT
  #if __GNUC__ >= 4
    #define UBLOX_F9R_PUBLIC __attribute__ ((visibility("default")))
    #define UBLOX_F9R_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define UBLOX_F9R_PUBLIC
    #define UBLOX_F9R_LOCAL
  #endif
  #define UBLOX_F9R_PUBLIC_TYPE
#endif
#endif  // UBLOX_F9R__VISIBILITY_CONTROL_H_
// Generated 16-Sep-2022 15:58:41
// Copyright 2019-2020 The MathWorks, Inc.
