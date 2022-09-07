# All Target
all: wasm_module1.js

# Tool invocations
wasm_module1.js: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Emscripten C++ Linker'
	em++ -s EXPORT_NAME=wasm_module1 -s MODULARIZE=1 -s USE_SDL=2 -s WASM=1 -g4 --bind -o "wasm_module1.js" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
