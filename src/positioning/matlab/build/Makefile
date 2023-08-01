#******************************************************************************
#
# Copyright (C) 2023 - 2028 KETI, All rights reserved.
#                           (Korea Electronics Technology Institute)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# Use of the Software is limited solely to applications:
# (a) running for Korean Government Project, or
# (b) that interact with KETI project/platform.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# KETI BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
# OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Except as contained in this notice, the name of the KETI shall not be used
# in advertising or otherwise to promote the sale, use or other dealings in
# this Software without prior written authorization from KETI.
#
#******************************************************************************

# Project path
CUR_DIR = $(shell pwd)
TOP_DIR = $(CUR_DIR)

# Source path
SRC_DIR = $(CUR_DIR)/../algorithm/codegen/lib
UWB_POS_LIB_PATH = $(SRC_DIR)/UWBpos
MAIN_PATH = $(SRC_DIR)/UWBpos/examples

# Include path
OUT_PATH = $(TOP_DIR)/out

# CFLAGS / LDFLAGS
TARGET_CFLAGS =
TARGET_CFLAGS += -I$(UWB_POS_LIB_PATH)
TARGET_CFLAGS += -I$(MAIN_PATH)
TARGET_CFLAGS += -D_GNU_SOURCE=1
TARGET_CFLAGS += -std=gnu99 -g
TARGET_CFLAGS += -Wall -Wextra -Wstrict-prototypes -O3 -fomit-frame-pointer -pipe
TARGET_CFLAGS += -std=c11
TARGET_LDFLAGS ?= -L$(OUT_PATH) -luwbpos -lpthread

#
# Compiler
#

TARGET_CC=gcc
TARGET_LD=ld
TARGET_AR=ar
TARGET_RANLIB=ranlib

# Advanced SIMD (aka NEON) is mandatory for AArch64, so no command line option is needed to instruct the compiler to use NEON.
# If you want to enable auto vectorization optimisations so that the compiler automatically uses NEON instructions, then compile with -O3 or -O2 -ftree-vectorize.
#TARGET_LDFLAGS+=-mcpu=cortex-a53 -O1 -O3 -Ofast -ffast-math
#TARGET_LDFLAGS+=-O2 -ftree-vectorize
#TARGET_LDFLAGS+=-mcpu=cortex-a53 -O3 -Ofast
#TARGET_LDFLAGS+=-mcpu=cortex-a53 -O3 -Ofast -flto -fno-signed-zeros -fno-trapping-math

TARGET_CONFIGS = \
	CC="$(TARGET_CC)" \
	LD="$(TARGET_LD)" \
	AR="$(TARGET_AR)" \
	RANLIB="$(TARGET_RANLIB)" \
	CFLAGS="$(TARGET_CFLAGS)" \
	LDFLAGS="$(TARGET_LDFLAGS)"
#
# UWB Positioning Library
#

uwbposlib:
	@$(MAKE) -C $(UWB_POS_LIB_PATH) $(TARGET_CONFIGS) all

uwbposlib-clean:
	$(MAKE) -C $(UWB_POS_LIB_PATH) clean

uwbposlib-distclean:
	$(MAKE) -C $(UWB_POS_LIB_PATH) distclean
	rm -f ./lib/*.a

.PHONY: uwbposlib uwbposlib-clean uwbposlib-distclean

#
# Main
#

main:
	@$(MAKE) -C $(MAIN_PATH) $(TARGET_CONFIGS) all

main-clean:
	$(MAKE) -C $(MAIN_PATH) clean

main-distclean:
	$(MAKE) -C $(MAIN_PATH) distclean
	rm -rf $(OUT_PATH)/*

.PHONY: main main-clean main-distclean

#
# Build All
#

all: uwbposlib main

#
# Build and Run
#

keti-run: distclean uwbposlib main
	$(OUT_PATH)/keti-app

#
# Clean
#

clean: distclean

distclean: uwbposlib-distclean main-distclean

