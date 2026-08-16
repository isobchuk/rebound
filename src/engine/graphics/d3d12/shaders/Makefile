DIR 		:= $(notdir $(CURDIR))
FULL_DIR	:= $(PRE_DIR)/$(DIR)

SHADER_FOLDER := $(ROOT)$(RESULT_FOLDER_NAME)/$(CONFIGURATION)/$(BIN_FOLDER_NAME)/shaders

SOURCES 	:= $(notdir $(wildcard ./*.hlsl))

CSO.VS := $(SOURCES:.hlsl=.vs.cso)
CSO.PS := $(SOURCES:.hlsl=.ps.cso)

CSO.VS 	:= $(addprefix $(SHADER_FOLDER)/, $(CSO.VS))
CSO.PS 	:= $(addprefix $(SHADER_FOLDER)/, $(CSO.PS))

SHADER.COMPILER := dxc

SHADER.VS.TYPE := vs_6_0 
SHADER.VS.ENTRY := VSMain

SHADER.PS.TYPE := ps_6_0 
SHADER.PS.ENTRY := PSMain

include $(ROOT)build/shader.mk