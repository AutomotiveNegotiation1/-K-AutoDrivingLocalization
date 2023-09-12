###########################################################################
## Makefile generated for component 'UWBpos2'. 
## 
## Makefile     : UWBpos2_rtw.mk
## Generated on : Fri Aug 11 11:15:18 2023
## Final product: ./UWBpos2.lib
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

PRODUCT_NAME              = UWBpos2
MAKEFILE                  = UWBpos2_rtw.mk
MATLAB_ROOT               = C:/PROGRA~1/MATLAB/R2022b
MATLAB_BIN                = C:/PROGRA~1/MATLAB/R2022b/bin
MATLAB_ARCH_BIN           = $(MATLAB_BIN)/win64
START_DIR                 = E:/MATLAB
TGT_FCN_LIB               = None
SOLVER_OBJ                = 
CLASSIC_INTERFACE         = 0
MODEL_HAS_DYNAMICALLY_LOADED_SFCNS = 
RELATIVE_PATH_TO_ANCHOR   = ../../..
CMD_FILE                  = UWBpos2_rtw.rsp
C_STANDARD_OPTS           = 
CPP_STANDARD_OPTS         = 
MODELLIB                  = UWBpos2.lib

###########################################################################
## TOOLCHAIN SPECIFICATIONS
###########################################################################

# Toolchain Name:          LCC-win64 v2.4.1 | gmake (64-bit Windows)
# Supported Version(s):    2.4.1
# ToolchainInfo Version:   2022b
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

PRODUCT = ./UWBpos2.lib
PRODUCT_TYPE = "static-library"
BUILD_TYPE = "Static Library"

###########################################################################
## INCLUDE PATHS
###########################################################################

INCLUDES_BUILDINFO = -I$(START_DIR)/codegen/lib/UWBpos2 -I$(START_DIR) -I$(MATLAB_ROOT)/extern/include

INCLUDES = $(INCLUDES_BUILDINFO)

###########################################################################
## DEFINES
###########################################################################

DEFINES_CUSTOM = 
DEFINES_STANDARD = -DMODEL=UWBpos2

DEFINES = $(DEFINES_CUSTOM) $(DEFINES_STANDARD)

###########################################################################
## SOURCE FILES
###########################################################################

SRCS = $(START_DIR)/codegen/lib/UWBpos2/UWBpos2_data.c $(START_DIR)/codegen/lib/UWBpos2/rt_nonfinite.c $(START_DIR)/codegen/lib/UWBpos2/rtGetNaN.c $(START_DIR)/codegen/lib/UWBpos2/rtGetInf.c $(START_DIR)/codegen/lib/UWBpos2/UWBpos2_initialize.c $(START_DIR)/codegen/lib/UWBpos2/UWBpos2_terminate.c $(START_DIR)/codegen/lib/UWBpos2/UWBpos2.c $(START_DIR)/codegen/lib/UWBpos2/sum.c $(START_DIR)/codegen/lib/UWBpos2/GetInitPos.c $(START_DIR)/codegen/lib/UWBpos2/inv.c $(START_DIR)/codegen/lib/UWBpos2/combineVectorElements.c $(START_DIR)/codegen/lib/UWBpos2/dec2bin.c $(START_DIR)/codegen/lib/UWBpos2/find.c $(START_DIR)/codegen/lib/UWBpos2/mean.c $(START_DIR)/codegen/lib/UWBpos2/GetPos3.c $(START_DIR)/codegen/lib/UWBpos2/TwoAnchPos3.c $(START_DIR)/codegen/lib/UWBpos2/sort.c $(START_DIR)/codegen/lib/UWBpos2/sortIdx.c $(START_DIR)/codegen/lib/UWBpos2/GetPosRefine2.c $(START_DIR)/codegen/lib/UWBpos2/UWBpos2_emxutil.c $(START_DIR)/codegen/lib/UWBpos2/UWBpos2_emxAPI.c $(START_DIR)/codegen/lib/UWBpos2/GetMultiTagPosGen2.c $(START_DIR)/codegen/lib/UWBpos2/UWBpos2_rtwutil.c

ALL_SRCS = $(SRCS)

###########################################################################
## OBJECTS
###########################################################################

OBJS = UWBpos2_data.obj rt_nonfinite.obj rtGetNaN.obj rtGetInf.obj UWBpos2_initialize.obj UWBpos2_terminate.obj UWBpos2.obj sum.obj GetInitPos.obj inv.obj combineVectorElements.obj dec2bin.obj find.obj mean.obj GetPos3.obj TwoAnchPos3.obj sort.obj sortIdx.obj GetPosRefine2.obj UWBpos2_emxutil.obj UWBpos2_emxAPI.obj GetMultiTagPosGen2.obj UWBpos2_rtwutil.obj

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


%.obj : $(START_DIR)/codegen/lib/UWBpos2/%.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


%.obj : $(START_DIR)/%.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


UWBpos2_data.obj : $(START_DIR)/codegen/lib/UWBpos2/UWBpos2_data.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


rt_nonfinite.obj : $(START_DIR)/codegen/lib/UWBpos2/rt_nonfinite.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


rtGetNaN.obj : $(START_DIR)/codegen/lib/UWBpos2/rtGetNaN.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


rtGetInf.obj : $(START_DIR)/codegen/lib/UWBpos2/rtGetInf.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


UWBpos2_initialize.obj : $(START_DIR)/codegen/lib/UWBpos2/UWBpos2_initialize.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


UWBpos2_terminate.obj : $(START_DIR)/codegen/lib/UWBpos2/UWBpos2_terminate.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


UWBpos2.obj : $(START_DIR)/codegen/lib/UWBpos2/UWBpos2.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


sum.obj : $(START_DIR)/codegen/lib/UWBpos2/sum.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


GetInitPos.obj : $(START_DIR)/codegen/lib/UWBpos2/GetInitPos.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


inv.obj : $(START_DIR)/codegen/lib/UWBpos2/inv.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


combineVectorElements.obj : $(START_DIR)/codegen/lib/UWBpos2/combineVectorElements.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


dec2bin.obj : $(START_DIR)/codegen/lib/UWBpos2/dec2bin.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


find.obj : $(START_DIR)/codegen/lib/UWBpos2/find.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


mean.obj : $(START_DIR)/codegen/lib/UWBpos2/mean.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


GetPos3.obj : $(START_DIR)/codegen/lib/UWBpos2/GetPos3.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


TwoAnchPos3.obj : $(START_DIR)/codegen/lib/UWBpos2/TwoAnchPos3.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


sort.obj : $(START_DIR)/codegen/lib/UWBpos2/sort.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


sortIdx.obj : $(START_DIR)/codegen/lib/UWBpos2/sortIdx.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


GetPosRefine2.obj : $(START_DIR)/codegen/lib/UWBpos2/GetPosRefine2.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


UWBpos2_emxutil.obj : $(START_DIR)/codegen/lib/UWBpos2/UWBpos2_emxutil.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


UWBpos2_emxAPI.obj : $(START_DIR)/codegen/lib/UWBpos2/UWBpos2_emxAPI.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


GetMultiTagPosGen2.obj : $(START_DIR)/codegen/lib/UWBpos2/GetMultiTagPosGen2.c
	$(CC) $(CFLAGS) -Fo"$@" $(subst /,\,"$<")


UWBpos2_rtwutil.obj : $(START_DIR)/codegen/lib/UWBpos2/UWBpos2_rtwutil.c
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
	$(ECHO) "### Deleting all derived files..."
	$(RM) $(subst /,\,$(PRODUCT))
	$(RM) $(subst /,\,$(ALL_OBJS))
	$(ECHO) "### Deleted all derived files."


