
#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

#include "stm32f4xx.h"

typedef enum
{  
    ERR_POS = 0x7F,
    LOW_POS =  1,
    MID_POS =  2,
    HIG_POS =  3
}PosFlag;

/*
图传指示灯，是座子第3脚，
闪烁是配置中；
常亮是连接正常；
常灭是连接异常或者没有配置；

*/


#define CLICK_NUM_MAX  10
typedef struct 
{
    PosFlag   level;
    PosFlag   level_last;
    u16  span_ms;               //计算多次按键统计时间，超出时间清除按键计数
    u32  click_ms[CLICK_NUM_MAX];       //按键次数
    u8   click_index;         //长时间不操作返回主页面
    u8   click1_flag;         //第一次按下标志
    u8   click2_flag;         //第二次按下标志
    u8   click3_flag;         //第三次按下标志  
    u8   click4_flag;         //第四次按下标志
    u8   click5_flag;         //第五次按下标志
    u8   click_clearFlag;     //按下标志清除标志位
    u32  clearFlag_ms;        //清空按下标志时间
    u32  count;
    u32  begin_ms;
    u32  systime_ms;
    u32  keeptime_ms;
    uint8_t  ContinuePressFlag;
} PartStates_releaseBackPos2_Struct_t;

typedef struct 
{
    PosFlag   level;
    PosFlag   level_last;
    u32  begin_ms;
    u32  systime_ms;
    u32  keeptime_ms;
} PartStates_pos3_Struct_t;

typedef struct 
{
    PosFlag   level;
    PosFlag   level_last;
    u32  begin_ms;
    u32  systime_ms;
    u32  keeptime_ms;
} PartStates_HoldonPos2_Struct_t;

typedef struct 
{
    u16   divCnt;
    u16   oneDiv;
    u16   pos;
} PartStates_knob_Struct_t;

typedef struct 
{   
    
    PartStates_HoldonPos2_Struct_t S1_pos2;
    PartStates_HoldonPos2_Struct_t S2_pos2;
    
    PartStates_releaseBackPos2_Struct_t K2_Pos2; 
    PartStates_releaseBackPos2_Struct_t K4_Pos2;     
    
    PartStates_releaseBackPos2_Struct_t K6_Pos2; 
    
    PartStates_releaseBackPos2_Struct_t K1_OK_Pos2;
    
    PartStates_releaseBackPos2_Struct_t K1_UP_Pos2;      //上
    PartStates_releaseBackPos2_Struct_t K1_DOWN_Pos2;    //下
    PartStates_releaseBackPos2_Struct_t K1_LEFT_Pos2;    //左
    PartStates_releaseBackPos2_Struct_t K1_RIGHT_Pos2;   //右
    PartStates_releaseBackPos2_Struct_t K1_FIR_ENTER_Pos2;    //首次确认键
    PartStates_releaseBackPos2_Struct_t K1_SEC_ENTER_Pos2;  //再次确认键
    
    PartStates_releaseBackPos2_Struct_t K1_BACK_Pos2;    //取消
    PartStates_releaseBackPos2_Struct_t K1_MENU_Pos2;    //菜单
    
    PartStates_releaseBackPos2_Struct_t K1_CALL_Pos2;   //呼叫
    PartStates_releaseBackPos2_Struct_t K1_MODO_Pos2;    //模式
    
    PartStates_releaseBackPos2_Struct_t K1_PREEMPT_Pos2; //控制权
    PartStates_releaseBackPos2_Struct_t K1_VIDEO_Pos2;   //视频通道切换
        
    PartStates_knob_Struct_t R10_knob; //多值旋钮
    PartStates_knob_Struct_t R9_knob; //多值旋钮
    
} PartsData_Struct_t;

extern PartsData_Struct_t  PartsData;



void GetSbusPartsStatesInit(void);

void RF_clickTest_ClearFlag(uint16_t time);
void ClearClickData(PartStates_releaseBackPos2_Struct_t *partx);
void GetParts_releaseBackPos2_States(PartStates_releaseBackPos2_Struct_t *partx,uint16_t channel);

void BSP_GPIO_Init(void);
void KeyScan(void);
void GetPartsStatesMultiFuction(void);
void Clear_All_GPIO_Input (void);

void Handle_video_trans_set(uint32_t set_timeout_ms);
#endif
































