################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/campservice.cpp \
../source/quothandler.cpp \
../source/quothandler_test.cpp \
../source/shared.cpp \
../source/shared_test.cpp \
../source/tradehandler.cpp \
../source/tradehandler_test.cpp \
../source/ull_qtp.cpp 

OBJS += \
./source/campservice.o \
./source/quothandler.o \
./source/quothandler_test.o \
./source/shared.o \
./source/shared_test.o \
./source/tradehandler.o \
./source/tradehandler_test.o \
./source/ull_qtp.o 

CPP_DEPS += \
./source/campservice.d \
./source/quothandler.d \
./source/quothandler_test.d \
./source/shared.d \
./source/shared_test.d \
./source/tradehandler.d \
./source/tradehandler_test.d \
./source/ull_qtp.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++1y -I../../libcli -I../../libutils -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


