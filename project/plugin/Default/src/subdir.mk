################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/browser.cc \
../src/browser_msg.cc \
../src/logger.cc \
../src/main.cc \
../src/mdnsbrowser.cc \
../src/npobject_browser.cc \
../src/plugin.cc \
../src/queue.cc 

OBJS += \
./src/browser.o \
./src/browser_msg.o \
./src/logger.o \
./src/main.o \
./src/mdnsbrowser.o \
./src/npobject_browser.o \
./src/plugin.o \
./src/queue.o 

CC_DEPS += \
./src/browser.d \
./src/browser_msg.d \
./src/logger.d \
./src/main.d \
./src/mdnsbrowser.d \
./src/npobject_browser.d \
./src/plugin.d \
./src/queue.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DXP_UNIX=1 -I/usr/include/dbus-1.0 -I/usr/lib/dbus-1.0/include -I/usr/include/xulrunner-1.9.1.4/stable -O2 -fPIC -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


