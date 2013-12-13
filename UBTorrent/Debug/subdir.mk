################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Announce.c \
../CalSHAHash.c \
../Getlocalip.c \
../MessageSend.c \
../Metainfo.c \
../PeerObject.c \
../Status.c \
../UBTorrent.c \
../bencode.c \
../readline.c \
../sha1.c 

OBJS += \
./Announce.o \
./CalSHAHash.o \
./Getlocalip.o \
./MessageSend.o \
./Metainfo.o \
./PeerObject.o \
./Status.o \
./UBTorrent.o \
./bencode.o \
./readline.o \
./sha1.o 

C_DEPS += \
./Announce.d \
./CalSHAHash.d \
./Getlocalip.d \
./MessageSend.d \
./Metainfo.d \
./PeerObject.d \
./Status.d \
./UBTorrent.d \
./bencode.d \
./readline.d \
./sha1.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


