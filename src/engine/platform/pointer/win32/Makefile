DIR 		:= $(notdir $(CURDIR))
FULL_DIR	:= $(PRE_DIR)/$(DIR)
INCLUDE_DIR ?= $(ROOT)/$(DIR)

OBJ_FOLDER 	:= $(ROOT)$(RESULT_FOLDER_NAME)/$(CONFIGURATION)/$(OBJ_FOLDER_NAME)$(FULL_DIR)
LIB_FOLDER 	:= $(ROOT)$(RESULT_FOLDER_NAME)/$(CONFIGURATION)/$(LIB_FOLDER_NAME)$(FULL_DIR)
BIN_FOLDER 	:= $(ROOT)$(RESULT_FOLDER_NAME)/$(CONFIGURATION)/$(BIN_FOLDER_NAME)$(FULL_DIR)

TARGET  	:= $(LIB_FOLDER)/lib$(DIR).lib

DIRS 		:= $(filter-out .,$(wildcard */))
INCLUDES 	:= $(notdir $(wildcard ./*.hpp))
SOURCES 	:= $(notdir $(wildcard ./*.cpp) $(wildcard ./*.c))
OBJECTS 	:= $(SOURCES:.cpp=.o)
OBJECTS 	:= $(addprefix $(OBJ_FOLDER)/, $(OBJECTS:.c=.o))

include $(ROOT)build/build.mk
