################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tempsensors_mod/tempsensors_mod.cpp \
../tempsensors_mod/wire_sensors.cpp 

OBJS += \
./tempsensors_mod/tempsensors_mod.o \
./tempsensors_mod/wire_sensors.o 

CPP_DEPS += \
./tempsensors_mod/tempsensors_mod.d \
./tempsensors_mod/wire_sensors.d 


# Each subdirectory must supply rules for building sources it contributes
tempsensors_mod/%.o: ../tempsensors_mod/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -std=c++1y -I"E:\PROGRAMOWANIE\OPROGRAMOWANIE\boost_1_61_0" -I"E:\PROGRAMOWANIE\BB_TOOLCHAIN\linarogcc\arm-linux-gnueabihf\libc\usr\include" -I"E:/PROGRAMOWANIE/BB_TOOLCHAIN/WORKSPACE/websockettele/websocketpp" -I"D:\Moje\boost_1_61_0" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


