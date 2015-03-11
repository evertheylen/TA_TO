################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../lib/tinystr.o \
../lib/tinyxml.o \
../lib/tinyxmlerror.o \
../lib/tinyxmlparser.o 

CPP_SRCS += \
../lib/tinystr.cpp \
../lib/tinyxml.cpp \
../lib/tinyxmlerror.cpp \
../lib/tinyxmlparser.cpp 

OBJS += \
./lib/tinystr.o \
./lib/tinyxml.o \
./lib/tinyxmlerror.o \
./lib/tinyxmlparser.o 

CPP_DEPS += \
./lib/tinystr.d \
./lib/tinyxml.d \
./lib/tinyxmlerror.d \
./lib/tinyxmlparser.d 


# Each subdirectory must supply rules for building sources it contributes
lib/%.o: ../lib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


