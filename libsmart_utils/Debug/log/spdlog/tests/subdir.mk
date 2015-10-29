################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../log/spdlog/tests/file_log.cpp \
../log/spdlog/tests/format.cpp \
../log/spdlog/tests/main.cpp \
../log/spdlog/tests/registry.cpp 

OBJS += \
./log/spdlog/tests/file_log.o \
./log/spdlog/tests/format.o \
./log/spdlog/tests/main.o \
./log/spdlog/tests/registry.o 

CPP_DEPS += \
./log/spdlog/tests/file_log.d \
./log/spdlog/tests/format.d \
./log/spdlog/tests/main.d \
./log/spdlog/tests/registry.d 


# Each subdirectory must supply rules for building sources it contributes
log/spdlog/tests/%.o: ../log/spdlog/tests/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++1y -I"/home/rock/github/test_repo/libcli" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


