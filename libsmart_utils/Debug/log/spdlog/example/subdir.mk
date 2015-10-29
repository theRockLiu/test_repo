################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../log/spdlog/example/bench.cpp \
../log/spdlog/example/example.cpp 

OBJS += \
./log/spdlog/example/bench.o \
./log/spdlog/example/example.o 

CPP_DEPS += \
./log/spdlog/example/bench.d \
./log/spdlog/example/example.d 


# Each subdirectory must supply rules for building sources it contributes
log/spdlog/example/%.o: ../log/spdlog/example/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++1y -I"/home/rock/github/test_repo/libcli" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


