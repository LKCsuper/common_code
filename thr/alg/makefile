VERSION   =
CC        = gcc
DEBUG     =
CFLAGS    = -fPIC -shared -DVERSION_INFO="Version $(VERSION)"
# 动态库
LFLAGS    = -fPIC -shared

# 源文件
SOURCES   = $(wildcard *.c)
SOURCES   += $(wildcard ./miracl_c/*.c)
SOURCES   += $(wildcard ./mode/*.c)

# 头文件
INCLUDES  = -I.
INCLUDES += -I./miracl_c
INCLUDES += -I./mode

LIB_NAMES =
LIB_PATH  =
OBJ       = $(patsubst %.c, %.o, $(SOURCES))
TARGET    = alg

# 链接
$(TARGET): $(OBJ)
	rm -rf ./output
	@mkdir -p output
	$(CC) $(OBJ) -L$(LIB_PATH) -l$(LIB_NAMES) $(LFLAGS) -o output/$(TARGET)$(VERSION).so
	ar rcs output/$(TARGET).a $(OBJ)  # 使用 ar 工具生成静态库
	@rm -rf $(OBJ)

# 编译
%.o: %.c
	$(CC) $(INCLUDES) $(DEBUG) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	@echo "删除链接和编译的文件..."
	rm -rf $(OBJ) $(TARGET) output
