################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/logger.cc \
../src/main.cc \
../src/plugin.cc \
../src/sb.cc 

OBJS += \
./src/logger.o \
./src/main.o \
./src/plugin.o \
./src/sb.o 

CC_DEPS += \
./src/logger.d \
./src/main.d \
./src/plugin.d \
./src/sb.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DXP_UNIX=1 -D_DEBUG -I/usr/include/xulrunner-1.9.1.5/stable -O0 -fPIC -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


