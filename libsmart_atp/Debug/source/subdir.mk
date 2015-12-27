################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/campservice.cpp \
../source/quothandler.cpp \
../source/shared.cpp \
../source/shared_test.cpp \
../source/tradehandler.cpp \
../source/ull_qtp.cpp 

OBJS += \
./source/campservice.o \
./source/quothandler.o \
./source/shared.o \
./source/shared_test.o \
./source/tradehandler.o \
./source/ull_qtp.o 

CPP_DEPS += \
./source/campservice.d \
./source/quothandler.d \
./source/shared.d \
./source/shared_test.d \
./source/tradehandler.d \
./source/ull_qtp.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++1y -I"/home/rock/github/test_repo/libsmart_utils" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


