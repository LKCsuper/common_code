# CC = gcc
# LD = gcc
# SRCS = $(wildcard *.c)
# OBJS = $(patsubst %c, %o, $(SRCS))
# # -I指定头文件目录
# INCLUDE = -I./include
# # -L指定库文件目录，-l指定静态库名字(去掉文件名中的lib前缀和.a后缀)
# LIB = -L./ -lpthread -lm
# # 开启编译warning和设置优化等级
# CFLAGS = -Wall -O2

# TARGET = speed

# .PHONY:all clean

# all: $(TARGET)
# # 链接时候指定库文件目录及库文件名
# $(TARGET): $(OBJS)
# 	$(LD) -o $@ $^ $(LIB)
 
# # 编译时候指定头文件目录
# %.o:%.c
# 	$(CC) -c $^ $(INCLUDE) $(CFLAGS) 

# clean:
# 	rm -f $(OBJS) $(TARGET)

# NOTE 配置交叉编译环境
CROSS_COMPILE ?=
# NOTE 配置架构
ARCH ?= $(shell uname -m | sed -e "s/i.86/i386/; s/ppc.*/powerpc/; s/armv.l/arm/; s/aarch64/arm64/; s/mips64el/mips/; s/loongarch64/loongarch/;")
# NOTE 配置内核目录
KDIR ?= /lib/modules/$(shell uname -r)/build

export CROSS_COMPILE ARCH KDIR

AS    ?=    $(Q)$(CROSS_COMPILE)as
CC    ?=    $(Q)$(CROSS_COMPILE)gcc
CXX   ?=    $(Q)$(CROSS_COMPILE)g++
LD    ?=    $(Q)$(CROSS_COMPILE)ld
AR    ?=    $(Q)$(CROSS_COMPILE)ar
SZ    ?=    $(Q)$(CROSS_COMPILE)size
OC    ?=    $(Q)$(CROSS_COMPILE)objcopy
OD    ?=    $(Q)$(CROSS_COMPILE)objdump

VERSION  =1.00
DEBUG   =-DUSE_DEBUG
CFLAGS  =-Wall -std=gnu99 -g
SOURCES   =$(wildcard ./*.c)
INCLUDES   =-I./
LIB_NAMES  =-lpthread -lm
LIB_PATH  =-L./
OBJ   =$(patsubst %.c, %.o, $(SOURCES))
TARGET  =speed
#links
$(TARGET):$(OBJ)
	@mkdir -p output
	$(CC) $(OBJ) $(LIB_PATH) $(LIB_NAMES) -o output/$(TARGET)
	@rm -rf $(OBJ)
 
#compile
%.o: %.c
	$(CC) $(INCLUDES) $(DEBUG) -c $(CFLAGS) $< -o $@
.PHONY:clean
clean:
	@echo "Remove linked and compiled files......"
	rm -rf $(OBJ) $(TARGET) output