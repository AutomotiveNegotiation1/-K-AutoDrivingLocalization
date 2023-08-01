###########################################################################
## Makefile generated for component 'TestTotal'. 
## 
## Makefile     : TestTotal_rtw.mk
## Generated on : Tue Aug 01 15:24:56 2023
## Final product: ./TestTotal.a
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

PRODUCT_NAME              = TestTotal
MAKEFILE                  = TestTotal_rtw.mk
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
MODELLIB                  = TestTotal.a

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

PRODUCT = ./TestTotal.a
PRODUCT_TYPE = "static-library"
BUILD_TYPE = "Static Library"

###########################################################################
## INCLUDE PATHS
###########################################################################

INCLUDES_BUILDINFO = -I$(START_DIR)/codegen/lib/TestTotal -I$(START_DIR) -I$(MATLAB_ROOT)/extern/include

INCLUDES = $(INCLUDES_BUILDINFO)

###########################################################################
## DEFINES
###########################################################################

DEFINES_CUSTOM = 
DEFINES_STANDARD = -DMODEL=TestTotal

DEFINES = $(DEFINES_CUSTOM) $(DEFINES_STANDARD)

###########################################################################
## SOURCE FILES
###########################################################################

SRCS = $(START_DIR)/codegen/lib/TestTotal/TestTotal_data.c $(START_DIR)/codegen/lib/TestTotal/rt_nonfinite.c $(START_DIR)/codegen/lib/TestTotal/rtGetNaN.c $(START_DIR)/codegen/lib/TestTotal/rtGetInf.c $(START_DIR)/codegen/lib/TestTotal/TestTotal_initialize.c $(START_DIR)/codegen/lib/TestTotal/TestTotal_terminate.c $(START_DIR)/codegen/lib/TestTotal/TestTotal.c $(START_DIR)/codegen/lib/TestTotal/randn.c $(START_DIR)/codegen/lib/TestTotal/rand.c $(START_DIR)/codegen/lib/TestTotal/sum.c $(START_DIR)/codegen/lib/TestTotal/find.c $(START_DIR)/codegen/lib/TestTotal/inv.c $(START_DIR)/codegen/lib/TestTotal/dec2bin.c $(START_DIR)/codegen/lib/TestTotal/minOrMax.c $(START_DIR)/codegen/lib/TestTotal/reverse.c $(START_DIR)/codegen/lib/TestTotal/mean.c $(START_DIR)/codegen/lib/TestTotal/mod.c $(START_DIR)/codegen/lib/TestTotal/eml_rand_mt19937ar_stateful.c $(START_DIR)/codegen/lib/TestTotal/UWBpos.c $(START_DIR)/codegen/lib/TestTotal/TwoAnchPos3.c $(START_DIR)/codegen/lib/TestTotal/exp.c $(START_DIR)/codegen/lib/TestTotal/eml_rand_mt19937ar.c $(START_DIR)/codegen/lib/TestTotal/GetPosRefine2.c $(START_DIR)/codegen/lib/TestTotal/TestTotal_rtwutil.c

ALL_SRCS = $(SRCS)

###########################################################################
## OBJECTS
###########################################################################

OBJS = TestTotal_data.o rt_nonfinite.o rtGetNaN.o rtGetInf.o TestTotal_initialize.o TestTotal_terminate.o TestTotal.o randn.o rand.o sum.o find.o inv.o dec2bin.o minOrMax.o reverse.o mean.o mod.o eml_rand_mt19937ar_stateful.o UWBpos.o TwoAnchPos3.o exp.o eml_rand_mt19937ar.o GetPosRefine2.o TestTotal_rtwutil.o

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


%.o : $(START_DIR)/codegen/lib/TestTotal/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.o : $(START_DIR)/codegen/lib/TestTotal/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


%.o : $(START_DIR)/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.o : $(START_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


TestTotal_data.o : $(START_DIR)/codegen/lib/TestTotal/TestTotal_data.c
	$(CC) $(CFLAGS) -o "$@" "$<"


rt_nonfinite.o : $(START_DIR)/codegen/lib/TestTotal/rt_nonfinite.c
	$(CC) $(CFLAGS) -o "$@" "$<"


rtGetNaN.o : $(START_DIR)/codegen/lib/TestTotal/rtGetNaN.c
	$(CC) $(CFLAGS) -o "$@" "$<"


rtGetInf.o : $(START_DIR)/codegen/lib/TestTotal/rtGetInf.c
	$(CC) $(CFLAGS) -o "$@" "$<"


TestTotal_initialize.o : $(START_DIR)/codegen/lib/TestTotal/TestTotal_initialize.c
	$(CC) $(CFLAGS) -o "$@" "$<"


TestTotal_terminate.o : $(START_DIR)/codegen/lib/TestTotal/TestTotal_terminate.c
	$(CC) $(CFLAGS) -o "$@" "$<"


TestTotal.o : $(START_DIR)/codegen/lib/TestTotal/TestTotal.c
	$(CC) $(CFLAGS) -o "$@" "$<"


randn.o : $(START_DIR)/codegen/lib/TestTotal/randn.c
	$(CC) $(CFLAGS) -o "$@" "$<"


rand.o : $(START_DIR)/codegen/lib/TestTotal/rand.c
	$(CC) $(CFLAGS) -o "$@" "$<"


sum.o : $(START_DIR)/codegen/lib/TestTotal/sum.c
	$(CC) $(CFLAGS) -o "$@" "$<"


find.o : $(START_DIR)/codegen/lib/TestTotal/find.c
	$(CC) $(CFLAGS) -o "$@" "$<"


inv.o : $(START_DIR)/codegen/lib/TestTotal/inv.c
	$(CC) $(CFLAGS) -o "$@" "$<"


dec2bin.o : $(START_DIR)/codegen/lib/TestTotal/dec2bin.c
	$(CC) $(CFLAGS) -o "$@" "$<"


minOrMax.o : $(START_DIR)/codegen/lib/TestTotal/minOrMax.c
	$(CC) $(CFLAGS) -o "$@" "$<"


reverse.o : $(START_DIR)/codegen/lib/TestTotal/reverse.c
	$(CC) $(CFLAGS) -o "$@" "$<"


mean.o : $(START_DIR)/codegen/lib/TestTotal/mean.c
	$(CC) $(CFLAGS) -o "$@" "$<"


mod.o : $(START_DIR)/codegen/lib/TestTotal/mod.c
	$(CC) $(CFLAGS) -o "$@" "$<"


eml_rand_mt19937ar_stateful.o : $(START_DIR)/codegen/lib/TestTotal/eml_rand_mt19937ar_stateful.c
	$(CC) $(CFLAGS) -o "$@" "$<"


UWBpos.o : $(START_DIR)/codegen/lib/TestTotal/UWBpos.c
	$(CC) $(CFLAGS) -o "$@" "$<"


TwoAnchPos3.o : $(START_DIR)/codegen/lib/TestTotal/TwoAnchPos3.c
	$(CC) $(CFLAGS) -o "$@" "$<"


exp.o : $(START_DIR)/codegen/lib/TestTotal/exp.c
	$(CC) $(CFLAGS) -o "$@" "$<"


eml_rand_mt19937ar.o : $(START_DIR)/codegen/lib/TestTotal/eml_rand_mt19937ar.c
	$(CC) $(CFLAGS) -o "$@" "$<"


GetPosRefine2.o : $(START_DIR)/codegen/lib/TestTotal/GetPosRefine2.c
	$(CC) $(CFLAGS) -o "$@" "$<"


TestTotal_rtwutil.o : $(START_DIR)/codegen/lib/TestTotal/TestTotal_rtwutil.c
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


