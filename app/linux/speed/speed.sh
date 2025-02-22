#!/bin/sh
###
 # @Description: 
 # @Version: 1.0
 # @Author: lkc
 # @Date: 2024-04-25 09:16:56
 # @LastEditors: Please set LastEditors
 # @LastEditTime: 2024-08-02 17:59:58
 # @detail: 
### 
TIMR=5                      # 测试时间
TIMES=3                     # 测试次数
THREAD=64                   # 线程数
PROCESS=process             # process or thread
DATA_LEN=4096
INDEX=0
TOOL=./output/speed
# 多线程测试

# sm1
$TOOL --type TEST_DEMO --$PROCESS $THREAD --time $TIMR --times $TIMES --len $DATA_LEN --head