/*****************************************************************************
*	模块名称： bsp_adc.c                                         	
*	程序功能：                             
*	编 写 者：                                   
*	编写日期：
                    
* 说明： 
****************************************************************************/

#include "adc.h"

#include "bsp_adc.h"
#include "bsp_gpio.h"
#include "bsp_lcd.h"

#include "app_flash.h"

struct_AdcParam  AdcParam;
struct_knob_time  Knob_Time;

void ADC_startConv(void)
{
  ADC_Cmd(ADC1, ENABLE);
  ADC_SoftwareStartConv(ADC1);    //开启ADC1连续转换
}

//                               -5  -4  -3  -2   -1   0     1    2    3    4
uint16_t RockerVolt_5gear[10] = {250,450,650,1000,1450,2650,3050,3350,3550,3700};
//                                0   1   2    3    4    5    6    7    8    9

//                              -4  -3  -2   -1   0     1    2    3    4
uint16_t RockerVolt_4gear[8] = {300,600,1000,1450,2650,3050,3450,3700};
//                              0   1   2    3    4    5    6    7    

//                              -3  -2   -1   0     1    2    3  
uint16_t RockerVolt_3gear[6] = {300,800,1450,2650,3250,3700};
//                                0   1   2    3    4    5  

//                              -2   -1   0     1    2 
uint16_t RockerVolt_2gear[4] = {400,1400,2700,3500};
//                                0   1   2    3   

//                               -1   0   1    
uint16_t RockerVolt_1gear[2] = {1300,2800};
//                                0   1  

#define UNUSED_FLAG 0x55

//通过判断adc通道读取数值来判断摇杆被按下的方向
int8_t checkRocker(uint16_t data ,uint8_t gear_num_max)
{
  int8_t gear  = 0;
  uint8_t i;
  
  if (data > 4096) {return UNUSED_FLAG;}
  
  switch(gear_num_max)
  {
    case 5: 
/*      
      if (data < RockerVolt_5gear[1]) {return -4;}
      if (data < RockerVolt_5gear[2]) {return -3;}
      if (data < RockerVolt_5gear[3]) {return -2;}
      if (data < RockerVolt_5gear[4]) {return -1;}
      if (data < RockerVolt_5gear[5]) {return 0;}
      if (data < RockerVolt_5gear[6]) {return 1;}
      if (data < RockerVolt_5gear[7]) {return 2;}
      if (data < RockerVolt_5gear[8]) {return 3;}
      if (data < RockerVolt_5gear[9]) {return 4;}  
*/       
      for(i=0;i<10;i++)
      {
        if (data < RockerVolt_5gear[i]) 
        {
          gear = i-5;
          return gear;
        }
      }   
      return 5;      
     break;
     
    case 4: 
      for(i=0;i<8;i++)
      {
        if (data < RockerVolt_4gear[i]) 
        {
          gear = i-4;
          return gear;
        }
      }   
      return 4;      
     break;     

    case 3: 
      for(i=0;i<6;i++)
      {
        if (data < RockerVolt_3gear[i]) 
        {
          gear = i-3;
          return gear;
        }
      }   
      return 3;      
     break;  
     
    case 2: 
      for(i=0;i<4;i++)
      {
        if (data < RockerVolt_2gear[i]) 
        {
          gear = i-2;
          return gear;
        }
      }   
      return 2;      
     break;   
    case 1: 
      for(i=0;i<2;i++)
      {
        if (data < RockerVolt_1gear[i]) 
        {
          gear = i-1;
          return gear;
        }
      }   
      return 1;      
     break;  
  }
  
  return UNUSED_FLAG;
}

void GetParam_UseAdcData(void)
{
    Get_Battery();
}

void Change_LKnob_ADC (void)
{
  
    if(Left_knob_Adc <= 512)
    {
        Knob_Time.UD_Power_Knob = 2;
    }
    else if(Left_knob_Adc >= 3583)
    {
        Knob_Time.UD_Power_Knob = 1;
    }
    else
    {
        Knob_Time.UD_Power_Knob = 0;
    }
    
    if(Right_knob_Adc <= 512)
    {
        Knob_Time.Close_Screen_Knob = 0;
    }
    else if(Right_knob_Adc >= 3583)
    {
        Knob_Time.Close_Screen_Knob = 1;
    }
    else{}
    
}


#define ADC_TO_VOLTAGE_PARAM  0.0088623  // = 1/4096*3.3*11
float bat_volt_HtoL[10] = {12.0,11.80,11.50,11.30,11.10,11.00,10.90,10.70,10.50,9.00};
void Get_Battery (void)
{
    uint16_t bat_adc = AdcParam.adcData[0];
    uint8_t  temp;
    
    AdcParam.bat_volt_current = bat_adc * ADC_TO_VOLTAGE_PARAM;   
    
    /* !NOTICE 实际电压有偏差大约低1v,默认偏移1v */
    AdcParam.bat_volt_current += 0.87;

    if(AdcParam.bat_volt_filter < 0.1)  //  类似于判断AdcParam.bat_volt_filter == 0；
    {
       AdcParam.bat_volt_filter = AdcParam.bat_volt_current;
    }
    else
    {
        AdcParam.bat_volt_filter = AdcParam.bat_volt_filter *0.9 + AdcParam.bat_volt_current*0.1;
    }
    
    
    AdcParam.bat_percent = 0;
    if(AdcParam.bat_volt_filter > bat_volt_HtoL[0]) AdcParam.bat_percent = 100;
    else
    {
        for(uint8_t i=1;i<10;i++)
        {
            if(AdcParam.bat_volt_filter > bat_volt_HtoL[i])
            {
                temp = (uint8_t)((AdcParam.bat_volt_filter - bat_volt_HtoL[i])/( bat_volt_HtoL[i-1]- bat_volt_HtoL[i])*10);
                AdcParam.bat_percent = (10-i)*10 + temp; 
                break;
            }        
        } 
    }    

    return;
}

