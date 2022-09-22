
# Makefile default goal
.DEFAULT_GOAL := build

# Makefile external parameters
ARCH_CXX?=g++
ARCH_AR?=ar
LIB_GDB?=
GDB_SYMBOLS?=0
OPT_LEVEL?=0

# Program to compiling C programs; default is `cc'
CXX=$(ARCH_CXX) # Builder of binaries
AR=$(ARCH_AR)   # Builder of static libraries

# Extra flags to give to the C++ compiler.
OPT_LEVEL_FLAG=-O$(OPT_LEVEL) # Optimization flag
ifeq ($(GDB_SYMBOLS),1)
    CXXFLAGS=-g -Wall $(OPT_LEVEL_FLAG)
else
    CXXFLAGS=-Wall $(OPT_LEVEL_FLAG)
endif

# Extra flags to give to compilers when they are supposed to invoke the linker, `ld'.
LDFLAGS=-lrt -std=c++17 -lpthread

MODULE_NAME=Rapidjson

# Directories of current library
DIR_INC=../inc
DIR_SRC=../src
DIR_OBJ=./obj
DIR_OUT=./out

DIR_INC_STRING=../../string/inc
DIR_INC_FILE=../../fs/file/inc
DIR_INC_EXTERNAL_RAPIDJSON=../../../external/rapidjson/inc

# Binary sources
SOURCES= $(wildcard $(DIR_SRC)/*.cpp)

# Binary objects
OBJECTS := $(addprefix $(DIR_OBJ)/, $(foreach obj,$(SOURCES:.cpp=.o),$(notdir $(obj))))

.PHONY: clean prescript postscript rebuild build

clean:
	@echo cleaning
	@rm -rf $(DIR_OBJ)/*.o
	@rm -rf $(DIR_OBJ)
	@rm -rf $(DIR_OUT)/*
	@rm -rf $(DIR_OUT)
	@echo cleaned

prescript:
	@echo building
	@mkdir -p $(DIR_OBJ)
	@mkdir -p $(DIR_OUT)

postscript:
	@echo built

rebuild: clean \
         build

build: prescript \
       $(DIR_OUT)/lib$(MODULE_NAME).a \
       postscript

# Create static library for current code
$(DIR_OUT)/lib$(MODULE_NAME).a: $(OBJECTS)
	$(AR) crv \
    $@ \
    $^ 
	# display
	$(AR) t $@
	# Indexation of the static library
	ranlib $@

# Create objects for current code
$(DIR_OBJ)/%.o: $(DIR_SRC)/%.cpp
	$(CXX) \
    -o $@ \
    -c $< \
    -I$(DIR_INC) \
    -I$(DIR_INC_STRING) \
    -I$(DIR_INC_FILE) \
    -I$(DIR_INC_EXTERNAL_RAPIDJSON) \
    $(CXXFLAGS)
