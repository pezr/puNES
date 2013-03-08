################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/apu.c \
../src/cpu.c \
../src/emu.c \
../src/external_calls.c \
../src/fds.c \
../src/fps.c \
../src/gamegenie.c \
../src/ines.c \
../src/input.c \
../src/irqA12.c \
../src/irql2f.c \
../src/main.c \
../src/mappers.c \
../src/ppu.c \
../src/save_slot.c \
../src/sha1.c \
../src/tas.c \
../src/timeline.c 

OBJS += \
./src/apu.o \
./src/cpu.o \
./src/emu.o \
./src/external_calls.o \
./src/fds.o \
./src/fps.o \
./src/gamegenie.o \
./src/ines.o \
./src/input.o \
./src/irqA12.o \
./src/irql2f.o \
./src/main.o \
./src/mappers.o \
./src/ppu.o \
./src/save_slot.o \
./src/sha1.o \
./src/tas.o \
./src/timeline.o 

C_DEPS += \
./src/apu.d \
./src/cpu.d \
./src/emu.d \
./src/external_calls.d \
./src/fds.d \
./src/fps.d \
./src/gamegenie.d \
./src/ines.d \
./src/input.d \
./src/irqA12.d \
./src/irql2f.d \
./src/main.d \
./src/mappers.d \
./src/ppu.d \
./src/save_slot.d \
./src/sha1.d \
./src/tas.d \
./src/timeline.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	i686-mingw32-gcc -DMINGW32 -DD3D9 -I../src -I../src/d3d9 -I../misc/DXSDK/Include -I/usr/i686-mingw32/mingw/include -O3 -Wall -ffast-math -c -fmessage-length=0 -finline-functions -Winline -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


