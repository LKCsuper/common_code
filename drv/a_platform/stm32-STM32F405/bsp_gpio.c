/*****************************************************************************
*	模块名称： bsp_gpio.c                                         	
*	程序功能：                             
*	编 写 者：                                   
*	编写日期：
                    
* 说明： 
****************************************************************************/


#include "gpio.h"

#include "bsp_gpio.h"
#include "bsp_tim.h"
#include "app_log.h"


PartsData_Struct_t  PartsData;

/*****************************************************************************

******************************************************************************/
uint8_t GetInput(uint8_t ch)
{
    //uint8_t i=0;

    return 0;
}

uint8_t   modeChange_last,GPIO_modeChange_last;

void GetInputALL(void)
{
    //uint8_t i = 0;
    //static uint32_t stop_time = 0;
    
}

void KeyScan(void)
{
  GetInputALL();
}


void GetParts_HoldonPos2_States(PartStates_HoldonPos2_Struct_t *partx,uint16_t channel)    //长按判断
{
/* *******  手松开保持当前状态值*/    
   if(channel == 0)
   {
      partx->level = LOW_POS; 
   }
   else if(channel == 1)
   {
      partx->level = HIG_POS;   
   }
   else
   {
      partx->level = ERR_POS; //相当于错误
   }
   
   if(partx->level != partx->level_last)
   {
      partx->level_last = partx->level;
      partx->begin_ms = TimParam.sysTime_ms; 
   }
   else
   {
      partx->systime_ms = TimParam.sysTime_ms;
      partx->keeptime_ms = TimParam.sysTime_ms - partx->begin_ms;   
   }
}


void GetParts_releaseBackPos2_States(PartStates_releaseBackPos2_Struct_t *partx,uint16_t channel)    //多次按下判断
{
/* *******     两位拨  手松开自动恢复*/ 
  
uint8_t pairIndex;

//    if(partx->click_clearFlag == 1) //刚刚处理完数据，x时间内容，执行的新动作无效，目的：我取单击flag，实际上迅速按两次也执行一次。
//    {
//        if((TimParam.sysTime_ms > partx->clearFlag_ms +100)||(TimParam.sysTime_ms < partx->clearFlag_ms))
//        {
//           partx->click_clearFlag = 0;
//        }
//        else
//        {
//          return;
//        }
//    }


    if(channel == 1)
   {
      partx->level = HIG_POS;    
   }
   else if(channel == 0)
   {
      partx->level = LOW_POS;   
   }
   else
   {
      partx->level = ERR_POS; //相当于错误
   }
   
   if(partx->click_index >= CLICK_NUM_MAX)
   {
      partx->click_index = 0;
   }
   
   if(partx->level != partx->level_last)
   {
      partx->begin_ms = TimParam.sysTime_ms;
      partx->level_last = partx->level;
      partx->click_ms[partx->click_index] = TimParam.sysTime_ms;
      if(partx->level == HIG_POS) 
      {
        partx->count++;
      }else{;}
      if(partx->click1_flag == 0)
      {
          pairIndex = (partx->click_index + CLICK_NUM_MAX-1)%CLICK_NUM_MAX; // 这次是1,上次是0， 这次是2，上次是1    
          if((partx->click_ms[partx->click_index] - partx->click_ms[pairIndex] < partx->span_ms)
             &&(partx->click_ms[pairIndex] != 0))  //=0 means this is the init data;
          {
             partx->click1_flag = 1;
          }
          else partx->click1_flag = 0;
      }
      
      if(partx->click2_flag == 0)
      {
          pairIndex = (partx->click_index + CLICK_NUM_MAX-3)%CLICK_NUM_MAX; // 这次是3，上次是0，这次是4，上次是1    
          if((partx->click_ms[partx->click_index] - partx->click_ms[pairIndex] < partx->span_ms)
             &&(partx->click_ms[pairIndex] != 0))
          {
            partx->click2_flag = 1;
          }
          else partx->click2_flag = 0;
      }
      
      if(partx->click3_flag == 0)
      {
          pairIndex = (partx->click_index + CLICK_NUM_MAX-5)%CLICK_NUM_MAX; // 这次是5，上次是0，这次是6，上次是1   
          if((partx->click_ms[partx->click_index] - partx->click_ms[pairIndex] < partx->span_ms)
             &&(partx->click_ms[pairIndex] != 0))
          {
            partx->click3_flag = 1;
          }
          else partx->click3_flag = 0;
      }
      
       if(partx->click4_flag == 0)
      {
          pairIndex = (partx->click_index + CLICK_NUM_MAX-7)%CLICK_NUM_MAX; // 这次是7，上次是0，这次是8，上次是1   
          if((partx->click_ms[partx->click_index] - partx->click_ms[pairIndex] < partx->span_ms)
            &&(partx->click_ms[pairIndex] != 0))
          {
            partx->click4_flag = 1;
          }
          else partx->click4_flag = 0;
      } 
      
      if(partx->click5_flag == 0)
      {
          pairIndex = (partx->click_index + CLICK_NUM_MAX -9)%CLICK_NUM_MAX; // 这次是9，上次是0  这次是8，上次是9      
          if((partx->click_ms[partx->click_index] - partx->click_ms[pairIndex] < partx->span_ms)
             &&(partx->click_ms[pairIndex] != 0))
          {
            partx->click5_flag = 1;
          }
          else partx->click5_flag = 0;
      }      
      partx->click_index++;
      if(partx->click_index >= CLICK_NUM_MAX)
      {
          partx->click_index = 0;
      }
   }
   else
   {
   
   }
      partx->systime_ms = TimParam.sysTime_ms;
      partx->keeptime_ms = TimParam.sysTime_ms - partx->begin_ms;
}


void GetParts_knob_States(PartStates_knob_Struct_t *partx,uint16_t channel)
{
/* *******     圆形旋钮 */ 
  
   if(partx->oneDiv == 0) return;  //旋钮参数没有初始化
  
   partx->pos = channel/partx->oneDiv;
   if(partx->pos >= partx->divCnt)
   {
     partx->pos = partx->divCnt - 1;
   }
   partx->pos = partx->divCnt - 1 - partx->pos;//纯粹为了调整方向变为顺时针是数据增大；
}


 void ClearClickData(PartStates_releaseBackPos2_Struct_t *partx)
{
  uint8_t i;
      partx->click_index = 0;
      for(i=0;i<CLICK_NUM_MAX;i++)
      {
          partx->click_ms[i] = 0;
      }
      partx->click1_flag = 0;
      partx->click2_flag = 0;
      partx->click3_flag = 0;
      partx->click4_flag = 0;
      partx->click5_flag = 0;
      partx->click_clearFlag = 1;
      partx->clearFlag_ms = TimParam.sysTime_ms;
}

struct_subTimParam  RF_clickTestTime;
void RF_clickTest_ClearFlag(uint16_t time)
{
	  if(GetTimeFlag(&RF_clickTestTime,time))
		{
        ClearClickData(&PartsData.K2_Pos2);        
    }
}

