# Project name
TARGET	= Rebound

# Compiler system
CC		= clang
CXX		= clang++
AR		= llvm-ar
SIZE	= llvm-size

# Global Defines for the different configurations (others can be added)
DEFINES = -DUNICODE
DEFINES_DEBUG =
DEFINES_RELEASE = -DNDEBUG

# Global flags for different configurations (others can be added)
CONFIGURATION ?= debug
ifeq ($(CONFIGURATION), release)
DEFINES 		+= $(DEFINES_RELEASE)
OPTIMIZE		= -Oz
SYMBOLS			=
else
DEFINES 		+= $(DEFINES_DEBUG)
OPTIMIZE		= -Og
SYMBOLS			= -g
endif

STANDARD = -std=c++23

# Folders with results
RESULT_FOLDER_NAME 		= output
BIN_FOLDER_NAME		   	= bin
LIB_FOLDER_NAME			= lib
OBJ_FOLDER_NAME			= obj

# Global warnings configuration
WARNINGS = \
    -Werror \
    -Wall \
    -Wextra \
    -Wpedantic \
    -Wcast-qual \
    -Wconversion \
    -Wsign-conversion \
    -Wshadow \
    -Wnon-virtual-dtor \
    -Woverloaded-virtual \
    -Wzero-as-null-pointer-constant \
    -Wold-style-cast \
    -Wimplicit-fallthrough \
    -Wnull-dereference \
    -Wundef \
    -Wunreachable-code \
    -Wmissing-field-initializers \
    -Wformat=2

WARNINGS_CPP = \
	-Wextra-semi \
    -Wdelete-non-virtual-dtor \
    -Wself-move \
    -Wpessimizing-move \
    -Wredundant-move \
    -Wdeprecated-copy \
    -Wreorder \
    -Wrange-loop-analysis

WARNINGS_NO = \

# Code directories
DIRS := src
GLOBAL_INCLUDE := $(CURDIR)/src

# Compilation flags and linker script
FLAGS		= $(SYMBOLS) $(OPTIMIZE) $(DEFINES) -I$(GLOBAL_INCLUDE)
CFLAGS		=
CXXFLAGS 	= $(STANDARD) -fdata-sections -ffunction-sections -fno-rtti -fno-exceptions
LDFLAGS		= -Wl,/MACHINE:X64 -Wl,/OPT:REF
LIBS		= -mwindows -luser32 -ld3d12

include $(ROOT)build/main.mk

