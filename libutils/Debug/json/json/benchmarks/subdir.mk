################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../json/json/benchmarks/benchmarks.cpp 

OBJS += \
./json/json/benchmarks/benchmarks.o 

CPP_DEPS += \
./json/json/benchmarks/benchmarks.d 


# Each subdirectory must supply rules for building sources it contributes
json/json/benchmarks/%.o: ../json/json/benchmarks/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


