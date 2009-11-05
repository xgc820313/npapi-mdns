################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/logger.cc \
../src/plugin.cc 

OBJS += \
./src/logger.o \
./src/plugin.o 

CC_DEPS += \
./src/logger.d \
./src/plugin.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DXP_UNIX=1 -D_DEBUG -I/usr/include/xulrunner-1.9.1.4/stable -O0 -fPIC -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


