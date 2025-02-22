
#ifndef __BSP_ADC_H__
#define __BSP_ADC_H__

#include "stm32f4xx.h"

#include "adc.h"

/**********************************************************************
get_adcData[0] PA0 IN0  S8  �Ҳ�ҡ������   ���ϱ��   ���±�С
get_adcData[1] PA1 IN1  S8  �Ҳ�ҡ������   �����С   ���ұ��
get_adcData[2] PA4 IN4  R9  �Ҳ���ť   ˳ʱ��
get_adcData[3] PB0 IN8  R10 �����ť   ˳ʱ��
get_adcData[4] PB1 IN9  K1  �������ȷ��>��>��>��>��
get_adcData[5] PC4 IN14 S9  ���ҡ������   �����С   ���ұ��
get_adcData[6] PC5 IN15 S9  ���ҡ������   ���ϱ��   ���±�С

ҡ�˵�ѹ���䣨����ֵ��
4096/9 = 455.111
    [0]  [1]   [2]   [3]   [4]  [5]   [6]   [7]    
0   455  910  1365  1820  2275  2730  3185  3640  4096
  4 || 3 || 2  || 1  || 0  || 1  || 2  || 3  ||  4
 
���򰴼���ѹ���䣨����ֵ��
4096/6 = 682.6666?-?^
0   683  1365  2048  2731  3413  4096 
    ��    ��    ��    ��   ȷ��
**********************************************************************/

typedef enum {
KEY_RELEASE  = 0x00,     //���򰴼���û������
KEY_DOWN,    //���򰴼�----��
KEY_RIGHT,   //���򰴼�----��
KEY_UP,      //���򰴼�----��
KEY_LEFT,    //���򰴼�----��
KEY_OK       //���򰴼�----ȷ��
}Key_Status;

#define Left_LR_Adc     AdcParam.adcData[3]
#define Left_UD_Adc     AdcParam.adcData[1]

#define Right_knob_Adc  0//AdcParam.adcData[0]
#define Left_knob_Adc   0//AdcParam.adcData[1]

#define Key5dir_Adc     0//AdcParam.adcData[0]

#define Right_LR_Adc    AdcParam.adcData[2]
#define Right_UD_Adc    AdcParam.adcData[0]

typedef enum {
 DIR_NULL = 0x00,
 DIR_FRONT =  0x01,
 DIR_BACK   =  0x02,
 DIR_LEFT  =   0x04,
 DIR_RIGHT =   0x08,
 DIR_UP    =   0x10,
 DIR_DOWN   =  0x20
}Tower_Dir;

#pragma pack(1)


/*
Left   rocker/gear   Left -  Right +   Up +   Down -

Right  rocker/gear  Up -   Down +

Left and right Knob   anti-clockwise  +    clockwise -

*/



typedef struct{
  
  uint16_t      adcData[ADC_CH_CNT];
  //	Tower_Dir    		Left_LR_dir;	
  int8_t   		Left_LR_gear; //���µ�λ----0~4
  int8_t   		Left_LR_gear_last; 
  uint32_t      Left_LR_keeptimes;	
  
  //	Tower_Dir    		Left_UD_dir;	
  int8_t   		Left_UD_gear; //���µ�λ----0~4
  int8_t   		Left_UD_gear_last; 
  uint32_t      Left_UD_keeptimes;	
  
  //	Tower_Dir    		Right_LR_dir;	
  int8_t   		Right_LR_gear;//���µ�λ----0~4
  int8_t   		Right_LR_gear_last; 	
  uint32_t      Right_LR_keeptimes;	
  
  //	Tower_Dir    		Right_UD_dir;	
  int8_t   		Right_UD_gear;//���µ�λ----0~4
  int8_t   		Right_UD_gear_last; 
  uint32_t      Right_UD_keeptimes;	
  
  uint16_t 	 		Left_knob; //�����ť----0~4096
  uint32_t      Left_knob_keeptimes;	
  
  uint16_t 	 		Right_knob; //�Ҳ���ť----0~4096
  uint32_t      Right_knob_keeptimes;	
  
  float bat_volt_current;//��ǰ��ص�ѹ
  float bat_volt_filter;//�˲���ص�ѹ
  uint8_t bat_percent;
  
}struct_AdcParam;

typedef struct{
  uint16_t     Close_Screen_Knob;     //��������ť
  uint16_t     UD_Power_Knob;     //���µ���ť
}struct_knob_time;

#pragma pack()

extern struct_AdcParam  AdcParam;
extern struct_knob_time  Knob_Time;


uint32_t GpioScan(void);




void ADC_startConv(void);
void GetParam_UseAdcData(void);
void Change_LKnob_ADC (void);
void Get_Battery (void);

#endif

