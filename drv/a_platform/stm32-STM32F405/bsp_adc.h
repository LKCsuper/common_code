
#ifndef __BSP_ADC_H__
#define __BSP_ADC_H__

#include "stm32f4xx.h"

#include "adc.h"

/**********************************************************************
get_adcData[0] PA0 IN0  S8  右侧摇杆上下   向上变大   向下变小
get_adcData[1] PA1 IN1  S8  右侧摇杆左右   向左变小   向右变大
get_adcData[2] PA4 IN4  R9  右侧旋钮   顺时针
get_adcData[3] PB0 IN8  R10 左侧旋钮   顺时针
get_adcData[4] PB1 IN9  K1  五项按键，确定>左>上>右>下
get_adcData[5] PC4 IN14 S9  左侧摇杆左右   向左变小   向右变大
get_adcData[6] PC5 IN15 S9  左侧摇杆上下   向上变大   向下变小

摇杆电压分配（理论值）
4096/9 = 455.111
    [0]  [1]   [2]   [3]   [4]  [5]   [6]   [7]    
0   455  910  1365  1820  2275  2730  3185  3640  4096
  4 || 3 || 2  || 1  || 0  || 1  || 2  || 3  ||  4
 
五向按键电压分配（理论值）
4096/6 = 682.6666?-?^
0   683  1365  2048  2731  3413  4096 
    下    右    上    左   确定
**********************************************************************/

typedef enum {
KEY_RELEASE  = 0x00,     //五向按键均没被按下
KEY_DOWN,    //五向按键----下
KEY_RIGHT,   //五向按键----右
KEY_UP,      //五向按键----上
KEY_LEFT,    //五向按键----左
KEY_OK       //五向按键----确定
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
  int8_t   		Left_LR_gear; //上下档位----0~4
  int8_t   		Left_LR_gear_last; 
  uint32_t      Left_LR_keeptimes;	
  
  //	Tower_Dir    		Left_UD_dir;	
  int8_t   		Left_UD_gear; //上下档位----0~4
  int8_t   		Left_UD_gear_last; 
  uint32_t      Left_UD_keeptimes;	
  
  //	Tower_Dir    		Right_LR_dir;	
  int8_t   		Right_LR_gear;//上下档位----0~4
  int8_t   		Right_LR_gear_last; 	
  uint32_t      Right_LR_keeptimes;	
  
  //	Tower_Dir    		Right_UD_dir;	
  int8_t   		Right_UD_gear;//上下档位----0~4
  int8_t   		Right_UD_gear_last; 
  uint32_t      Right_UD_keeptimes;	
  
  uint16_t 	 		Left_knob; //左侧旋钮----0~4096
  uint32_t      Left_knob_keeptimes;	
  
  uint16_t 	 		Right_knob; //右侧旋钮----0~4096
  uint32_t      Right_knob_keeptimes;	
  
  float bat_volt_current;//当前电池电压
  float bat_volt_filter;//滤波电池电压
  uint8_t bat_percent;
  
}struct_AdcParam;

typedef struct{
  uint16_t     Close_Screen_Knob;     //开关屏旋钮
  uint16_t     UD_Power_Knob;     //上下电旋钮
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

