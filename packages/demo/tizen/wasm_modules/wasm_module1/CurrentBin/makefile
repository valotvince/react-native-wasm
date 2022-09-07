################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables
EMSCRIPTEN_OUTPUT_OUTPUTS += \
wasm_module1.wasm.map \
wasm_module1.worker.js \
wasm_module1.wasm \
wasm_module1.wast \
wasm_module1.js.mem \
wasm_module1.html \

# All Target
all: wasm_module1.js

# Tool invocations
wasm_module1.js: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Emscripten C++ Linker'
	em++ -s EXPORT_NAME=wasm_module1 -s MODULARIZE=1 -s USE_SDL=2 -s WASM=1 -g4 --bind -o "wasm_module1.js" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

wasm_module1.wasm.map: wasm_module1.js $(OBJS) $(USER_OBJS)
wasm_module1.worker.js: wasm_module1.js $(OBJS) $(USER_OBJS)
wasm_module1.wasm: wasm_module1.js $(OBJS) $(USER_OBJS)
wasm_module1.wast: wasm_module1.js $(OBJS) $(USER_OBJS)
wasm_module1.js.mem: wasm_module1.js $(OBJS) $(USER_OBJS)
wasm_module1.html: wasm_module1.js $(OBJS) $(USER_OBJS)

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EMSCRIPTEN_OUTPUT_OUTPUTS)$(EXECUTABLES)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(C_DEPS)$(CPP_DEPS) wasm_module1.js
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
