.PHONY: all build run clean

TARGET = main
BUILD_DIR = build

RM = rm -rf
MKDIR = mkdir -p
RUN_CMD = ./$(BUILD_DIR)/$(TARGET).exe
ARG := $(word 2, $(MAKECMDGOALS))

all: build run

$(BUILD_DIR)/Makefile:
	@$(MKDIR) $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -G "MinGW Makefiles" ..

build: $(BUILD_DIR)/Makefile
	cd $(BUILD_DIR) && mingw32-make

run:
	@echo "Ejecutando Lab: $(ARG)"
	@$(RUN_CMD) $(ARG)

clean:
	@$(RM) $(BUILD_DIR)


%:
	@: