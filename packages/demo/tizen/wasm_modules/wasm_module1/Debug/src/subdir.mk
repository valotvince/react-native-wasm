################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/functions.cpp \
../src/helloworld.cpp 

OBJS += \
./src/functions.o \
./src/helloworld.o 

CPP_DEPS += \
./src/functions.d \
./src/helloworld.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Emscripten C++ Compiler'
	em++ -O0 -g4 -Wall -c -fmessage-length=0  -I../inc -Wall -g4 -std=gnu++11 --bind -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


