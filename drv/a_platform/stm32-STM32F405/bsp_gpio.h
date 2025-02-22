
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
ͼ��ָʾ�ƣ������ӵ�3�ţ�
��˸�������У�
����������������
�����������쳣����û�����ã�

*/


#define CLICK_NUM_MAX  10
typedef struct 
{
    PosFlag   level;
    PosFlag   level_last;
    u16  span_ms;               //�����ΰ���ͳ��ʱ�䣬����ʱ�������������
    u32  click_ms[CLICK_NUM_MAX];       //��������
    u8   click_index;         //��ʱ�䲻����������ҳ��
    u8   click1_flag;         //��һ�ΰ��±�־
    u8   click2_flag;         //�ڶ��ΰ��±�־
    u8   click3_flag;         //�����ΰ��±�־  
    u8   click4_flag;         //���Ĵΰ��±�־
    u8   click5_flag;         //����ΰ��±�־
    u8   click_clearFlag;     //���±�־�����־λ
    u32  clearFlag_ms;        //��հ��±�־ʱ��
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
    
    PartStates_releaseBackPos2_Struct_t K1_UP_Pos2;      //��
    PartStates_releaseBackPos2_Struct_t K1_DOWN_Pos2;    //��
    PartStates_releaseBackPos2_Struct_t K1_LEFT_Pos2;    //��
    PartStates_releaseBackPos2_Struct_t K1_RIGHT_Pos2;   //��
    PartStates_releaseBackPos2_Struct_t K1_FIR_ENTER_Pos2;    //�״�ȷ�ϼ�
    PartStates_releaseBackPos2_Struct_t K1_SEC_ENTER_Pos2;  //�ٴ�ȷ�ϼ�
    
    PartStates_releaseBackPos2_Struct_t K1_BACK_Pos2;    //ȡ��
    PartStates_releaseBackPos2_Struct_t K1_MENU_Pos2;    //�˵�
    
    PartStates_releaseBackPos2_Struct_t K1_CALL_Pos2;   //����
    PartStates_releaseBackPos2_Struct_t K1_MODO_Pos2;    //ģʽ
    
    PartStates_releaseBackPos2_Struct_t K1_PREEMPT_Pos2; //����Ȩ
    PartStates_releaseBackPos2_Struct_t K1_VIDEO_Pos2;   //��Ƶͨ���л�
        
    PartStates_knob_Struct_t R10_knob; //��ֵ��ť
    PartStates_knob_Struct_t R9_knob; //��ֵ��ť
    
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
































