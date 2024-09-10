# HW 정보
ARCH = armv7-a 
MCPU = cortex-a8 
# 크로스 컴파일러 실행 파일
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as 
LD = arm-none-eabi-ld 
OC = arm-none-eabi-objcopy
# 링커 스크립트
LINKER_SCRIPT = ./navilos.ld
# 어셈블리 파일, object 파일 목록
ASM_SRCS = $(wildcard boot/*.S) # 확장자가 S인 파일을 ARM_SRCS 변수에 넣음
ASM_OBJS = $(patsubst boot/%.S, build/%.o, $(ASM_SRCS)) # S 확장자 파일을 o확장자로 바꾸어 ASM OBJS에 넣음
# 최종 목표인 ELF 파일.
navilos = build/navilos.axf
navilos_bin = build/navilos.bin 

.PHONY : all clean run debug gdb

all : $(navilos)

clean :
	@rm -fr build

run : $(navilos)
	qemu-system-arm -M realview-pb-a8 -kernel $(navilos)

debug: $(navilos)
	qemu-system-arm -M realview-pb-a8 -kernel $(navilos) -S -gdb tcp::1234,ipv4

# arm-none-eabi-gdb
gdb:
	gdb-multiarch
# 링커로 navilos.axf파일 생성함.
$(navilos) : $(ASM_OBJS) $(LINKER_SCRIPT)
	$(LD) -n -T $(LINKER_SCRIPT) -o $(navilos) $(ASM_OBJS)
	$(OC) -O binary $(navilos) $(navilos_bin)

build/%.o : boot/%.S 
	mkdir -p $(shell dirname $@)
	$(AS) -march=$(ARCH) -mcpu=$(MCPU) -g -o $@ $<