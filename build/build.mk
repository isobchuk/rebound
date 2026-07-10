include $(ROOT)build/log.mk

.PHONY: all clean $(DIRS)

all: $(DIRS) $(OBJ_FOLDER) $(LIB_FOLDER) $(TARGET)
	@:

$(TARGET): $(OBJECTS)
	@$(LOG_AR)
	$(AR) rcs $@ $^
	
$(DIRS):
	$(MAKE) -C $@ ROOT=../$(ROOT) PRE_DIR=$(FULL_DIR) $(MAKECMDGOALS) CC=$(CC) CXX=$(CXX) AR=$(AR)

$(OBJ_FOLDER)/%.o: %.cpp
	@$(LOG_CXX)
	$(CXX) $(FLAGS) $(CXXFLAGS) $(WARNINGS) $(WARNINGS_CPP) $(WARNINGS_NO) -MMD -c $< -o $@

$(OBJ_FOLDER)/%.o: %.c
	@$(LOG_CC)
	$(CC) $(FLAGS) $(CFLAGS) $(WARNINGS) $(WARNINGS_NO) -MMD -c $< -o $@

$(OBJ_FOLDER):
	$(shell mkdir "$(OBJ_FOLDER)")

$(LIB_FOLDER):
	$(shell mkdir "$(LIB_FOLDER)")

include $(wildcard $(OBJECTS:.o=.d))

clean: $(DIRS)
	@:
	$(shell rmdir /s /q  "$(OBJ_FOLDER)")
	$(shell rmdir /s /q  "$(LIB_FOLDER)")