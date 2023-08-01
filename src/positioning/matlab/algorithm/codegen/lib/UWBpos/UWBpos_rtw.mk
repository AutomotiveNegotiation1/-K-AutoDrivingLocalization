###########################################################################
## Makefile generated for component 'UWBpos'. 
## 
## Makefile     : UWBpos_rtw.mk
## Generated on : Tue Aug 01 14:36:03 2023
## Final product: ./UWBpos.a
## Product type : static-library
## 
###########################################################################

###########################################################################
## MACROS
###########################################################################

# Macro Descriptions:
# PRODUCT_NAME            Name of the system to build
# MAKEFILE                Name of this makefile
# MODELLIB                Static library target

PRODUCT_NAME              = UWBpos
MAKEFILE                  = UWBpos_rtw.mk
MATLAB_ROOT               = /usr/local/MATLAB/R2023a
MATLAB_BIN                = /usr/local/MATLAB/R2023a/bin
MATLAB_ARCH_BIN           = $(MATLAB_BIN)/glnxa64
START_DIR                 = /home/bman/work/position/src/positioning/matlab/algorithm
TGT_FCN_LIB               = ISO_C
SOLVER_OBJ                = 
CLASSIC_INTERFACE         = 0
MODEL_HAS_DYNAMICALLY_LOADED_SFCNS = 
RELATIVE_PATH_TO_ANCHOR   = ../../..
C_STANDARD_OPTS           = -fwrapv
CPP_STANDARD_OPTS         = -fwrapv
MODELLIB                  = UWBpos.a

###########################################################################
## TOOLCHAIN SPECIFICATIONS
###########################################################################

# Toolchain Name:          GNU gcc/g++ | gmake (64-bit Linux)
# Supported Version(s):    
# ToolchainInfo Version:   2023a
# Specification Revision:  1.0
# 
#-------------------------------------------
# Macros assumed to be defined elsewhere
#-------------------------------------------

# C_STANDARD_OPTS
# CPP_STANDARD_OPTS

#-----------
# MACROS
#-----------

WARN_FLAGS         = -Wall -W -Wwrite-strings -Winline -Wstrict-prototypes -Wnested-externs -Wpointer-arith -Wcast-align
WARN_FLAGS_MAX     = $(WARN_FLAGS) -Wcast-qual -Wshadow
CPP_WARN_FLAGS     = -Wall -W -Wwrite-strings -Winline -Wpointer-arith -Wcast-align
CPP_WARN_FLAGS_MAX = $(CPP_WARN_FLAGS) -Wcast-qual -Wshadow

TOOLCHAIN_SRCS = 
TOOLCHAIN_INCS = 
TOOLCHAIN_LIBS = 

#------------------------
# BUILD TOOL COMMANDS
#------------------------

# C Compiler: GNU C Compiler
CC = gcc

# Linker: GNU Linker
LD = g++

# C++ Compiler: GNU C++ Compiler
CPP = g++

# C++ Linker: GNU C++ Linker
CPP_LD = g++

# Archiver: GNU Archiver
AR = ar

# MEX Tool: MEX Tool
MEX_PATH = $(MATLAB_ARCH_BIN)
MEX = "$(MEX_PATH)/mex"

# Download: Download
DOWNLOAD =

# Execute: Execute
EXECUTE = $(PRODUCT)

# Builder: GMAKE Utility
MAKE_PATH = %MATLAB%/bin/glnxa64
MAKE = "$(MAKE_PATH)/gmake"


#-------------------------
# Directives/Utilities
#-------------------------

CDEBUG              = -g
C_OUTPUT_FLAG       = -o
LDDEBUG             = -g
OUTPUT_FLAG         = -o
CPPDEBUG            = -g
CPP_OUTPUT_FLAG     = -o
CPPLDDEBUG          = -g
OUTPUT_FLAG         = -o
ARDEBUG             =
STATICLIB_OUTPUT_FLAG =
MEX_DEBUG           = -g
RM                  = @rm -f
ECHO                = @echo
MV                  = @mv
RUN                 =

#--------------------------------------
# "Faster Runs" Build Configuration
#--------------------------------------

ARFLAGS              = ruvs
CFLAGS               = -c $(C_STANDARD_OPTS) -fPIC \
                       -O3 -fno-loop-optimize -fno-aggressive-loop-optimizations
CPPFLAGS             = -c $(CPP_STANDARD_OPTS) -fPIC \
                       -O3 -fno-loop-optimize -fno-aggressive-loop-optimizations
CPP_LDFLAGS          =
CPP_SHAREDLIB_LDFLAGS  = -shared -Wl,--no-undefined
DOWNLOAD_FLAGS       =
EXECUTE_FLAGS        =
LDFLAGS              =
MEX_CPPFLAGS         =
MEX_CPPLDFLAGS       =
MEX_CFLAGS           =
MEX_LDFLAGS          =
MAKE_FLAGS           = -f $(MAKEFILE)
SHAREDLIB_LDFLAGS    = -shared -Wl,--no-undefined



###########################################################################
## OUTPUT INFO
###########################################################################

PRODUCT = ./UWBpos.a
PRODUCT_TYPE = "static-library"
BUILD_TYPE = "Static Library"

###########################################################################
## INCLUDE PATHS
###########################################################################

INCLUDES_BUILDINFO = -I$(START_DIR)/codegen/lib/UWBpos -I$(START_DIR) -I$(MATLAB_ROOT)/extern/include

INCLUDES = $(INCLUDES_BUILDINFO)

###########################################################################
## DEFINES
###########################################################################

DEFINES_CUSTOM = 
DEFINES_STANDARD = -DMODEL=UWBpos

DEFINES = $(DEFINES_CUSTOM) $(DEFINES_STANDARD)

###########################################################################
## SOURCE FILES
###########################################################################

SRCS = $(START_DIR)/codegen/lib/UWBpos/UWBpos_data.c $(START_DIR)/codegen/lib/UWBpos/rt_nonfinite.c $(START_DIR)/codegen/lib/UWBpos/rtGetNaN.c $(START_DIR)/codegen/lib/UWBpos/rtGetInf.c $(START_DIR)/codegen/lib/UWBpos/UWBpos_initialize.c $(START_DIR)/codegen/lib/UWBpos/UWBpos_terminate.c $(START_DIR)/codegen/lib/UWBpos/UWBpos.c $(START_DIR)/codegen/lib/UWBpos/sum.c $(START_DIR)/codegen/lib/UWBpos/find.c $(START_DIR)/codegen/lib/UWBpos/GetInitPos.c $(START_DIR)/codegen/lib/UWBpos/inv.c $(START_DIR)/codegen/lib/UWBpos/combineVectorElements.c $(START_DIR)/codegen/lib/UWBpos/dec2bin.c $(START_DIR)/codegen/lib/UWBpos/mean.c $(START_DIR)/codegen/lib/UWBpos/GetPos2.c $(START_DIR)/codegen/lib/UWBpos/TwoAnchPos3.c $(START_DIR)/codegen/lib/UWBpos/UWBpos_emxutil.c $(START_DIR)/codegen/lib/UWBpos/UWBpos_emxAPI.c $(START_DIR)/codegen/lib/UWBpos/UWBpos_rtwutil.c

ALL_SRCS = $(SRCS)

###########################################################################
## OBJECTS
###########################################################################

OBJS = UWBpos_data.o rt_nonfinite.o rtGetNaN.o rtGetInf.o UWBpos_initialize.o UWBpos_terminate.o UWBpos.o sum.o find.o GetInitPos.o inv.o combineVectorElements.o dec2bin.o mean.o GetPos2.o TwoAnchPos3.o UWBpos_emxutil.o UWBpos_emxAPI.o UWBpos_rtwutil.o

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

SYSTEM_LIBS =  -lm

###########################################################################
## ADDITIONAL TOOLCHAIN FLAGS
###########################################################################

#---------------
# C Compiler
#---------------

CFLAGS_BASIC = $(DEFINES) $(INCLUDES)

CFLAGS += $(CFLAGS_BASIC)

#-----------------
# C++ Compiler
#-----------------

CPPFLAGS_BASIC = $(DEFINES) $(INCLUDES)

CPPFLAGS += $(CPPFLAGS_BASIC)

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
	$(AR) $(ARFLAGS)  $(PRODUCT) $(OBJS)
	@echo "### Created: $(PRODUCT)"


###########################################################################
## INTERMEDIATE TARGETS
###########################################################################

#---------------------
# SOURCE-TO-OBJECT
#---------------------

%.o : %.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.o : %.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


%.o : $(RELATIVE_PATH_TO_ANCHOR)/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.o : $(RELATIVE_PATH_TO_ANCHOR)/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


%.o : $(START_DIR)/codegen/lib/UWBpos/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.o : $(START_DIR)/codegen/lib/UWBpos/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


%.o : $(START_DIR)/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.o : $(START_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


UWBpos_data.o : $(START_DIR)/codegen/lib/UWBpos/UWBpos_data.c
	$(CC) $(CFLAGS) -o "$@" "$<"


rt_nonfinite.o : $(START_DIR)/codegen/lib/UWBpos/rt_nonfinite.c
	$(CC) $(CFLAGS) -o "$@" "$<"


rtGetNaN.o : $(START_DIR)/codegen/lib/UWBpos/rtGetNaN.c
	$(CC) $(CFLAGS) -o "$@" "$<"


rtGetInf.o : $(START_DIR)/codegen/lib/UWBpos/rtGetInf.c
	$(CC) $(CFLAGS) -o "$@" "$<"


UWBpos_initialize.o : $(START_DIR)/codegen/lib/UWBpos/UWBpos_initialize.c
	$(CC) $(CFLAGS) -o "$@" "$<"


UWBpos_terminate.o : $(START_DIR)/codegen/lib/UWBpos/UWBpos_terminate.c
	$(CC) $(CFLAGS) -o "$@" "$<"


UWBpos.o : $(START_DIR)/codegen/lib/UWBpos/UWBpos.c
	$(CC) $(CFLAGS) -o "$@" "$<"


sum.o : $(START_DIR)/codegen/lib/UWBpos/sum.c
	$(CC) $(CFLAGS) -o "$@" "$<"


find.o : $(START_DIR)/codegen/lib/UWBpos/find.c
	$(CC) $(CFLAGS) -o "$@" "$<"


GetInitPos.o : $(START_DIR)/codegen/lib/UWBpos/GetInitPos.c
	$(CC) $(CFLAGS) -o "$@" "$<"


inv.o : $(START_DIR)/codegen/lib/UWBpos/inv.c
	$(CC) $(CFLAGS) -o "$@" "$<"


combineVectorElements.o : $(START_DIR)/codegen/lib/UWBpos/combineVectorElements.c
	$(CC) $(CFLAGS) -o "$@" "$<"


dec2bin.o : $(START_DIR)/codegen/lib/UWBpos/dec2bin.c
	$(CC) $(CFLAGS) -o "$@" "$<"


mean.o : $(START_DIR)/codegen/lib/UWBpos/mean.c
	$(CC) $(CFLAGS) -o "$@" "$<"


GetPos2.o : $(START_DIR)/codegen/lib/UWBpos/GetPos2.c
	$(CC) $(CFLAGS) -o "$@" "$<"


TwoAnchPos3.o : $(START_DIR)/codegen/lib/UWBpos/TwoAnchPos3.c
	$(CC) $(CFLAGS) -o "$@" "$<"


UWBpos_emxutil.o : $(START_DIR)/codegen/lib/UWBpos/UWBpos_emxutil.c
	$(CC) $(CFLAGS) -o "$@" "$<"


UWBpos_emxAPI.o : $(START_DIR)/codegen/lib/UWBpos/UWBpos_emxAPI.c
	$(CC) $(CFLAGS) -o "$@" "$<"


UWBpos_rtwutil.o : $(START_DIR)/codegen/lib/UWBpos/UWBpos_rtwutil.c
	$(CC) $(CFLAGS) -o "$@" "$<"


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
	@echo "### CPPFLAGS = $(CPPFLAGS)"
	@echo "### CPP_LDFLAGS = $(CPP_LDFLAGS)"
	@echo "### CPP_SHAREDLIB_LDFLAGS = $(CPP_SHAREDLIB_LDFLAGS)"
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
	$(RM) $(PRODUCT)
	$(RM) $(ALL_OBJS)
	$(ECHO) "### Deleted all derived files."


