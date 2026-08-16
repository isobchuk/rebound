include $(ROOT)build/log.mk

.PHONY: all clean

all: $(SHADER_FOLDER) $(CSO.VS) $(CSO.PS)
	@:

$(SHADER_FOLDER)/%.vs.cso: %.hlsl
	@$(LOG_DXC)
	$(SHADER.COMPILER) -T $(SHADER.VS.TYPE) -E $(SHADER.VS.ENTRY) $< -Fo $@

$(SHADER_FOLDER)/%.ps.cso: %.hlsl
	@$(LOG_DXC)
	$(SHADER.COMPILER) -T $(SHADER.PS.TYPE) -E $(SHADER.PS.ENTRY) $< -Fo $@

$(SHADER_FOLDER):
	$(shell mkdir "$(SHADER_FOLDER)")

clean:
	@:
	$(shell rmdir /s /q  "$(SHADER_FOLDER)")
