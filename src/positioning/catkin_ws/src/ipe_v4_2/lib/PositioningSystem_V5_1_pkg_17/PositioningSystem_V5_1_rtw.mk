###########################################################################
## Makefile generated for component 'PositioningSystem_V5_1'. 
## 
## Makefile     : PositioningSystem_V5_1_rtw.mk
## Generated on : Fri Aug 23 16:25:03 2024
## Final product: ./PositioningSystem_V5_1.lib
## Product type : static-library
## 
###########################################################################

###########################################################################
## MACROS
###########################################################################

# Macro Descriptions:
# PRODUCT_NAME            Name of the system to build
# MAKEFILE                Name of this makefile
# CMD_FILE                Command file
# MODELLIB                Static library target

PRODUCT_NAME              = PositioningSystem_V5_1
MAKEFILE                  = PositioningSystem_V5_1_rtw.mk
MATLAB_ROOT               = C:/PROGRA~1/MATLAB/R2023b
MATLAB_BIN                = C:/PROGRA~1/MATLAB/R2023b/bin
MATLAB_ARCH_BIN           = $(MATLAB_BIN)/win64
START_DIR                 = C:/Users/KETI/Documents/MATLAB/Positioning_Alg_20240724_pkg5_1_12_original/Positioning_Alg_20240724_pkg5_1_12/Positioning_Alg_20240724_pkg5_1_12
TGT_FCN_LIB               = None
SOLVER_OBJ                = 
CLASSIC_INTERFACE         = 0
MODEL_HAS_DYNAMICALLY_LOADED_SFCNS = 
RELATIVE_PATH_TO_ANCHOR   = ../../..
CMD_FILE                  = PositioningSystem_V5_1_rtw.rsp
C_STANDARD_OPTS           = 
CPP_STANDARD_OPTS         = 
MODELLIB                  = PositioningSystem_V5_1.lib

###########################################################################
## TOOLCHAIN SPECIFICATIONS
###########################################################################

# Toolchain Name:          LCC-win64 v2.4.1 | gmake (64-bit Windows)
# Supported Version(s):    2.4.1
# ToolchainInfo Version:   2023b
# Specification Revision:  1.0
# 

#-----------
# MACROS
#-----------

SHELL              = cmd
LCC_ROOT           = $(MATLAB_ROOT)/sys/lcc64/lcc64
LCC_BUILDLIB       = $(LCC_ROOT)/bin/buildlib
LCC_LIB            = $(LCC_ROOT)/lib64
MW_EXTERNLIB_DIR   = $(MATLAB_ROOT)/extern/lib/win64/microsoft
MW_LIB_DIR         = $(MATLAB_ROOT)/lib/win64
TOOLCHAIN_INCLUDES = -I$(LCC_ROOT)/include64

TOOLCHAIN_SRCS = 
TOOLCHAIN_INCS = 
TOOLCHAIN_LIBS = 

#------------------------
# BUILD TOOL COMMANDS
#------------------------

# C Compiler: Lcc-win64 C Compiler
CC_PATH = $(LCC_ROOT)/bin
CC = "$(CC_PATH)/lcc64"

# Linker: Lcc-win64 Linker
LD_PATH = $(LCC_ROOT)/bin
LD = "$(LD_PATH)/lcclnk64"

# Archiver: Lcc-win64 Archiver
AR_PATH = $(LCC_ROOT)/bin
AR = "$(AR_PATH)/lcclib64"

# MEX Tool: MEX Tool
MEX_PATH = $(MATLAB_ARCH_BIN)
MEX = "$(MEX_PATH)/mex"

# Download: Download
DOWNLOAD =

# Execute: Execute
EXECUTE = $(PRODUCT)

# Builder: GMAKE Utility
MAKE_PATH = %MATLAB%\bin\win64
MAKE = "$(MAKE_PATH)/gmake"


#-------------------------
# Directives/Utilities
#-------------------------

CDEBUG              = -g
C_OUTPUT_FLAG       = -Fo
LDDEBUG             =
OUTPUT_FLAG         = -o
ARDEBUG             =
STATICLIB_OUTPUT_FLAG = /out:
MEX_DEBUG           = -g
RM                  = @del /F
ECHO                = @echo
MV                  = @move
RUN                 =

#--------------------------------------
# "Faster Runs" Build Configuration
#--------------------------------------

ARFLAGS              =
CFLAGS               = -c -w -noregistrylookup -nodeclspec -I$(LCC_ROOT)/include64
DOWNLOAD_FLAGS       =
EXECUTE_FLAGS        =
LDFLAGS              = -s -L$(LCC_LIB) $(LDFLAGS_ADDITIONAL)
MEX_CPPFLAGS         =
MEX_CPPLDFLAGS       =
MEX_CFLAGS           =
MEX_LDFLAGS          =
MAKE_FLAGS           = -f $(MAKEFILE)
SHAREDLIB_LDFLAGS    = -dll -entry LibMain -s -L$(LCC_LIB) $(LDFLAGS_ADDITIONAL) $(DEF_FILE)



###########################################################################
## OUTPUT INFO
###########################################################################

PRODUCT = ./PositioningSystem_V5_1.lib
PRODUCT_TYPE = "static-library"
BUILD_TYPE = "Static Library"

###########################################################################
## INCLUDE PATHS
###########################################################################

INCLUDES_BUILDINFO = -I$(START_DIR)/codegen/lib/PositioningSystem_V5_1 -I$(START_DIR) -I$(MATLAB_ROOT)/extern/include

INCLUDES = $(INCLUDES_BUILDINFO)

###########################################################################
## DEFINES
###########################################################################

DEFINES_CUSTOM = 
DEFINES_STANDARD = -DMODEL=PositioningSystem_V5_1

DEFINES = $(DEFINES_CUSTOM) $(DEFINES_STANDARD)

###########################################################################
## SOURCE FILES
###########################################################################

SRCS = $(START_DIR)/codegen/lib/PositioningSystem_V5_1/PositioningSystem_V5_1_data.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/rt_nonfinite.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/rtGetNaN.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/rtGetInf.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/PositioningSystem_V5_1_initialize.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/PositioningSystem_V5_1_terminate.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/PositioningSystem_V5_1.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/interp1.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/equPlane.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/mod.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/EKF_UWB_SLAM_4.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/mod1.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/UWBPosition_V4_1.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/UWBpos_V2_3.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/inv.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/mldivide.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/abs.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/UWBMultiTagPos_V3_1.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/find.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/mean.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/EKF_UWB_SLAM_IMU_1.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/makePredA_3D_Simple.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/xzgetrf.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/exp.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/nullAssignment.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/sort.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/sortIdx.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/div.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/PositioningSystem_V5_1_emxutil.c $(START_DIR)/codegen/lib/PositioningSystem_V5_1/PositioningSystem_V5_1_rtwutil.c

ALL_SRCS = $(SRCS)

###########################################################################
## OBJECTS
###########################################################################

OBJS = PositioningSystem_V5_1_data.obj rt_nonfinite.obj rtGetNaN.obj rtGetInf.obj PositioningSystem_V5_1_initialize.obj PositioningSystem_V5_1_terminate.obj PositioningSystem_V5_1.obj interp1.obj equPlane.obj mod.obj EKF_UWB_SLAM_4.obj mod1.obj UWBPosition_V4_1.obj UWBpos_V2_3.obj inv.obj mldivide.obj abs.obj UWBMultiTagPos_V3_1.obj find.obj mean.obj EKF_UWB_SLAM_IMU_1.obj makePredA_3D_Simple.obj xzgetrf.obj exp.obj nullAssignment.obj sort.obj sortIdx.obj div.obj PositioningSystem_V5_1_emxutil.obj PositioningSystem_V5_1_rtwutil.obj

ALL_OBJS = $(OBJS)

###########################################################################
## PREBUILT OBJECT FILES
###########################################################################

PREBUILT_OBJS = 

###########################################################################
## LIBRARIES
###########################################################################

LIBS = 

###########################################################################
## SYSTEM LIBRARIES
###########################################################################

SYSTEM_LIBS = 

###########################################################################
## ADDITIONAL TOOLCHAIN FLAGS
###########################################################################

#---------------
# C Compiler
#---------------

CFLAGS_BASIC = $(DEFINES) $(INCLUDES)

CFLAGS += $(CFLAGS_BASIC)

###########################################################################
## INLINED COMMANDS
###########################################################################

###########################################################################
## PHONY TARGETS
###########################################################################

.PHONY : all build clean info prebuild download execute


all : build
	@echo "### Successfully generated all binary outputs."


build : prebuild $(PRODUCT)


prebuild : 


download : $(PRODUCT)


execute : download


###########################################################################
## FINAL TARGET
###########################################################################

#---------------------------------
# Create a static library         
#---------------------------------

$(PRODUCT) : $(OBJS) $(PREBUILT_OBJS)
	@echo "### Creating static library "$(PRODUCT)" ..."
	$(AR) $(ARFLAGS) /out:$(PRODUCT) @$(CMD_FILE)
	@echo "### Created: $(PRODUCT)"


###########################################################################
## INTERMEDIATE TARGETS
###########################################################################

#---------------------
# SOURCE-TO-OBJECT
#---------------------

%.obj : %.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


%.obj : $(RELATIVE_PATH_TO_ANCHOR)/%.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


%.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/%.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


%.obj : $(START_DIR)/%.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


PositioningSystem_V5_1_data.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/PositioningSystem_V5_1_data.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


rt_nonfinite.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/rt_nonfinite.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


rtGetNaN.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/rtGetNaN.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


rtGetInf.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/rtGetInf.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


PositioningSystem_V5_1_initialize.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/PositioningSystem_V5_1_initialize.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


PositioningSystem_V5_1_terminate.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/PositioningSystem_V5_1_terminate.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


PositioningSystem_V5_1.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/PositioningSystem_V5_1.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


interp1.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/interp1.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


equPlane.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/equPlane.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


mod.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/mod.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


EKF_UWB_SLAM_4.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/EKF_UWB_SLAM_4.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


mod1.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/mod1.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


UWBPosition_V4_1.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/UWBPosition_V4_1.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


UWBpos_V2_3.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/UWBpos_V2_3.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


inv.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/inv.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


mldivide.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/mldivide.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


abs.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/abs.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


UWBMultiTagPos_V3_1.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/UWBMultiTagPos_V3_1.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


find.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/find.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


mean.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/mean.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


EKF_UWB_SLAM_IMU_1.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/EKF_UWB_SLAM_IMU_1.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


makePredA_3D_Simple.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/makePredA_3D_Simple.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


xzgetrf.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/xzgetrf.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


exp.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/exp.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


nullAssignment.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/nullAssignment.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


sort.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/sort.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


sortIdx.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/sortIdx.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


div.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/div.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


PositioningSystem_V5_1_emxutil.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/PositioningSystem_V5_1_emxutil.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


PositioningSystem_V5_1_rtwutil.obj : $(START_DIR)/codegen/lib/PositioningSystem_V5_1/PositioningSystem_V5_1_rtwutil.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


###########################################################################
## DEPENDENCIES
###########################################################################

$(ALL_OBJS) : rtw_proj.tmw $(MAKEFILE)


###########################################################################
## MISCELLANEOUS TARGETS
###########################################################################

info : 
	@echo "### PRODUCT = $(PRODUCT)"
	@echo "### PRODUCT_TYPE = $(PRODUCT_TYPE)"
	@echo "### BUILD_TYPE = $(BUILD_TYPE)"
	@echo "### INCLUDES = $(INCLUDES)"
	@echo "### DEFINES = $(DEFINES)"
	@echo "### ALL_SRCS = $(ALL_SRCS)"
	@echo "### ALL_OBJS = $(ALL_OBJS)"
	@echo "### LIBS = $(LIBS)"
	@echo "### MODELREF_LIBS = $(MODELREF_LIBS)"
	@echo "### SYSTEM_LIBS = $(SYSTEM_LIBS)"
	@echo "### TOOLCHAIN_LIBS = $(TOOLCHAIN_LIBS)"
	@echo "### CFLAGS = $(CFLAGS)"
	@echo "### LDFLAGS = $(LDFLAGS)"
	@echo "### SHAREDLIB_LDFLAGS = $(SHAREDLIB_LDFLAGS)"
	@echo "### ARFLAGS = $(ARFLAGS)"
	@echo "### MEX_CFLAGS = $(MEX_CFLAGS)"
	@echo "### MEX_CPPFLAGS = $(MEX_CPPFLAGS)"
	@echo "### MEX_LDFLAGS = $(MEX_LDFLAGS)"
	@echo "### MEX_CPPLDFLAGS = $(MEX_CPPLDFLAGS)"
	@echo "### DOWNLOAD_FLAGS = $(DOWNLOAD_FLAGS)"
	@echo "### EXECUTE_FLAGS = $(EXECUTE_FLAGS)"
	@echo "### MAKE_FLAGS = $(MAKE_FLAGS)"


clean : 
	$(ECHO) "### Deleting all derived files ..."
	$(RM) $(subst /,\,$(PRODUCT))
	$(RM) $(subst /,\,$(ALL_OBJS))
	$(ECHO) "### Deleted all derived files."


