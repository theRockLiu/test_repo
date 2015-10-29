################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../log/spdlog/include/spdlog/details/format.cc 

CC_DEPS += \
./log/spdlog/include/spdlog/details/format.d 

OBJS += \
./log/spdlog/include/spdlog/details/format.o 


# Each subdirectory must supply rules for building sources it contributes
log/spdlog/include/spdlog/details/%.o: ../log/spdlog/include/spdlog/details/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++1y -I"/home/rock/github/test_repo/libcli" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


