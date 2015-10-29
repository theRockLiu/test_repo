################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../log/spdlog/bench/boost-bench-mt.cpp \
../log/spdlog/bench/boost-bench.cpp \
../log/spdlog/bench/easylogging-bench-mt.cpp \
../log/spdlog/bench/easylogging-bench.cpp \
../log/spdlog/bench/g2log-async.cpp \
../log/spdlog/bench/glog-bench-mt.cpp \
../log/spdlog/bench/glog-bench.cpp \
../log/spdlog/bench/spdlog-async.cpp \
../log/spdlog/bench/spdlog-bench-mt.cpp \
../log/spdlog/bench/spdlog-bench.cpp 

OBJS += \
./log/spdlog/bench/boost-bench-mt.o \
./log/spdlog/bench/boost-bench.o \
./log/spdlog/bench/easylogging-bench-mt.o \
./log/spdlog/bench/easylogging-bench.o \
./log/spdlog/bench/g2log-async.o \
./log/spdlog/bench/glog-bench-mt.o \
./log/spdlog/bench/glog-bench.o \
./log/spdlog/bench/spdlog-async.o \
./log/spdlog/bench/spdlog-bench-mt.o \
./log/spdlog/bench/spdlog-bench.o 

CPP_DEPS += \
./log/spdlog/bench/boost-bench-mt.d \
./log/spdlog/bench/boost-bench.d \
./log/spdlog/bench/easylogging-bench-mt.d \
./log/spdlog/bench/easylogging-bench.d \
./log/spdlog/bench/g2log-async.d \
./log/spdlog/bench/glog-bench-mt.d \
./log/spdlog/bench/glog-bench.d \
./log/spdlog/bench/spdlog-async.d \
./log/spdlog/bench/spdlog-bench-mt.d \
./log/spdlog/bench/spdlog-bench.d 


# Each subdirectory must supply rules for building sources it contributes
log/spdlog/bench/%.o: ../log/spdlog/bench/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++1y -I"/home/rock/github/test_repo/libcli" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


