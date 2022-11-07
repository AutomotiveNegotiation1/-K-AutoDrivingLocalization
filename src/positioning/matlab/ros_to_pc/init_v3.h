/*
 * MATLAB Compiler: 8.4 (R2022a)
 * Date: Wed Sep  7 11:51:17 2022
 * Arguments: "-B""macro_default""init_v3.m""-l"
 */

#ifndef init_v3_h
#define init_v3_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#ifdef __cplusplus
extern "C" { // sbcheck:ok:extern_c
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_init_v3_C_API 
#define LIB_init_v3_C_API /* No special import/export declaration */
#endif

/* GENERAL LIBRARY FUNCTIONS -- START */

extern LIB_init_v3_C_API 
bool MW_CALL_CONV init_v3InitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_init_v3_C_API 
bool MW_CALL_CONV init_v3Initialize(void);

extern LIB_init_v3_C_API 
void MW_CALL_CONV init_v3Terminate(void);

extern LIB_init_v3_C_API 
void MW_CALL_CONV init_v3PrintStackTrace(void);

/* GENERAL LIBRARY FUNCTIONS -- END */

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_init_v3_C_API 
bool MW_CALL_CONV mlxInit_v3(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

/* C INTERFACE -- MLX WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

/* C INTERFACE -- MLF WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- START */

extern LIB_init_v3_C_API bool MW_CALL_CONV mlfInit_v3();

#ifdef __cplusplus
}
#endif
/* C INTERFACE -- MLF WRAPPERS FOR USER-DEFINED MATLAB FUNCTIONS -- END */

#endif
