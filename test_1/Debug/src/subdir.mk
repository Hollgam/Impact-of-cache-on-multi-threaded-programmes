################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/mach_gettime.o \
../src/test_1.o 

C_SRCS += \
../src/mach_gettime.c \
../src/test_1.c 

OBJS += \
./src/mach_gettime.o \
./src/test_1.o 

C_DEPS += \
./src/mach_gettime.d \
./src/test_1.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


