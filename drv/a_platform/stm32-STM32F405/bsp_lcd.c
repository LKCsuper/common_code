/*****************************************************************************
*	模块名称： bsp_lcd.c                                         	
*	程序功能：                             
*	编 写 者：                                   
*	编写日期：
                    
* 说明： 
****************************************************************************/


#include "bsp_adc.h"
#include "bsp_lcd.h"
#include "bsp_tim.h"

#include "app_monitor.h"
#include "app_TClink_MRC.h"
#include "app_TClink_Mctrl.h"
#include "app_gps.h"
#include "oled.h"

#include "E22_400T22.h"

struct_LcdParam LcdParam;
struct_OLED_Display_Status  OLED_Display_Status;


//电量
unsigned  char  Battery_Power[11][16] = {
  //电池电量显示8X16
  {0x18,0x7E,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x7E,0x00},/*"电量0",0*/  

  {0x18,0x7E,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x5A,0x42,0x7E,0x00},/*"电量10"%",0*/

  {0x18,0x7E,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x5A,0x5A,0x42,0x7E,0x00},/*"电量20"%",0*/

  {0x18,0x7E,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x5A,0x5A,0x5A,0x42,0x7E,0x00},/*"电量30"%",0*/

  {0x18,0x7E,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x5A,0x5A,0x5A,0x5A,0x42,0x7E,0x00},/*"电量40"%",0*/

  {0x18,0x7E,0x42,0x42,0x42,0x42,0x42,0x42,0x5A,0x5A,0x5A,0x5A,0x5A,0x42,0x7E,0x00},/*"电量50"%",0*/

  {0x18,0x7E,0x42,0x42,0x42,0x42,0x42,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A,0x42,0x7E,0x00},/*"电量60"%",0*/

  {0x18,0x7E,0x42,0x42,0x42,0x42,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A,0x42,0x7E,0x00},/*"电量70"%",0*/

  {0x18,0x7E,0x42,0x42,0x42,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A,0x42,0x7E,0x00},/*"电量80"%",0*/

  {0x18,0x7E,0x42,0x42,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A,0x42,0x7E,0x00},/*"电量90"%",0*/

  {0x18,0x7E,0x42,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A,0x42,0x7E,0x00}/*"电量100"%",0*/
 
};

//信号显示16X8列扫描
unsigned  char  Lora_Rssi[7][16] = {
  
{0x01,0xFF,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, //无信号 
{0x01,0xFF,0x01,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//信号满1格
{0x01,0xFF,0x01,0xFC,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//信号满2格
{0x01,0xFF,0x01,0xFC,0x00,0xF8,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//信号满3格
{0x01,0xFF,0x01,0xFC,0x00,0xF8,0x00,0xF0,0x00,0xE0,0x00,0x00,0x00,0x00,0x00,0x00},//信号满4格 
{0x01,0xFF,0x01,0xFC,0x00,0xF8,0x00,0xF0,0x00,0xE0,0x00,0xC0,0x00,0x00,0x00,0x00},//信号满5格 
{0x01,0xFF,0x01,0xFC,0x00,0xF8,0x00,0xF0,0x00,0xE0,0x00,0xC0,0x00,0x80,0x00,0x00} //信号满6格 

};

////信号显示16X8行扫描[test]
//unsigned  char  Lora_Rssi[7][16] = {
//  
//{0x07,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00}, //无信号 
//{0x07,0x00,0x02,0x00,0x0A,0x00,0x0A,0x00,0x0A,0x00,0x0A,0x00,0x0A,0x00,0x0A,0x00},//信号满1格
//{0x07,0x00,0x02,0x00,0x0A,0x00,0x2A,0x00,0x2A,0x00,0x2A,0x00,0x2A,0x00,0x2A,0x00},//信号满2格
//{0x07,0x00,0x02,0x00,0x0A,0x00,0x2A,0x00,0xAA,0x00,0xAA,0x00,0xAA,0x00,0xAA,0x00},//信号满3格
//{0x07,0x00,0x02,0x00,0x0A,0x00,0x2A,0x00,0xAA,0x00,0xAA,0x02,0xAA,0x02,0xAA,0x02},//信号满4格 
//{0x07,0x00,0x02,0x00,0x0A,0x00,0x2A,0x00,0xAA,0x00,0xAA,0x02,0xAA,0x0A,0xAA,0x0A},//信号满5格 
//{0x07,0x00,0x02,0x00,0x0A,0x00,0x2A,0x00,0xAA,0x00,0xAA,0x02,0xAA,0x0A,0xAA,0x2A} //信号满6格 
//
//};

void Show_BataVolt_Chinese(float voltage,u16 x,u16 y,u16 sizey,u8 mode);

//显示急停列表
void OLED_DisPlay_EmergeStopList(uint8_t i,uint8_t num)
{
    if(i <= 10)
    {    
        OLED_ShowNum(82+(i-1)*16,15,num,2,16,0);
    }
    if((i > 10)&&(i <= 16))
    {
       OLED_ShowNum(8*(i-9)+(i-6)*16,32,num,2,16,0);
    }
}
struct_subTimParam oledTimParam;
void OLED_display_update(void)
{
//   uint8_t pos = 10,EmergeStoptemp ,StopNum = 0;
   uint32_t temp;
   
   static uint8_t index = 0;
   
   int8_t gear_LR;
   int8_t gear_FB;
   int8_t gear_UD;

   //急停显示
   if(KeyParam.EmergeStop)
   {
      OLED_ShowChinese(108,15,"急停已按下",16,0);
      OLED_ShowChinese(124,32,"请解除",16,0);
      return ;
   }
   if(GetTimeFlag(&oledTimParam,200))
   {   
       index++;
       if(index >2) index = 0;
       if(SysCtrl_param.iap_flag == 1)
       {
          index = 2;
       }
       
       if(fromTCBD_data.msg1.ctrlRights == CTRL_RIGHT_AUTO)
       {
          gear_LR =  fromTCBD_data.msg1.gear_angle  ;  
          gear_FB =  fromTCBD_data.msg1.gear_distance ;
          gear_UD =  fromTCBD_data.msg1.gear_height   ;
       }  
       else
       {
           gear_LR =  AdcParam.Left_LR_gear;
           gear_FB =  AdcParam.Left_UD_gear;
           gear_UD =  AdcParam.Right_UD_gear;
       } 
       
       switch(index)
       {
             case 0:  
               OLED_ShowChinese(68,34,"变幅",16,0);
               OLED_ShowString(100,34,":",16,0);   
               if(gear_FB>=0)//内向外向【变幅】
               {
                  OLED_ShowString(108,34," ",16,0);
                  OLED_ShowNum(116,34,gear_FB,1,16,0);
               }
               else
               {
                  temp = -gear_FB;          
                  OLED_ShowString(108,34,"-",16,0);
                  OLED_ShowNum(116,34,temp,1,16,0);
               }
               
               OLED_ShowChinese(128,34,"回转",16,0);
               OLED_ShowString(160,34,":",16,0);  
               if(gear_LR>=0)//左旋右旋【回转】
               {
                  OLED_ShowString(168,34," ",16,0);
                  OLED_ShowNum(176,34,gear_LR,1,16,0);
               }
               else
               {
                  temp = -gear_LR;          
                  OLED_ShowString(168,34,"-",16,0);
                  OLED_ShowNum(176,34,temp,1,16,0);
               }
             
               
               OLED_ShowChinese(188,34,"升降",16,0);
               OLED_ShowString(220,34,":",16,0);  
               if(gear_UD>=0)//升降
               {
                  OLED_ShowString(228,34," ",16,0);
                  OLED_ShowNum(236,34,gear_UD,1,16,0);
               }
               else
               {
                  temp = -gear_UD;          
                  OLED_ShowString(228,34,"-",16,0);
                  OLED_ShowNum(236,34,temp,1,16,0);
               }
       break;
       case 1:       
            temp = - E22_param.dbmNoiseRssi;
            if(temp > 99) temp =99;
            OLED_ShowRssi(54,15,temp,16,0);
           
            temp = - E22_RSSI_param.dbmDataRssi_from_MCTRL;       
            if(temp > 99) temp =99;
            if(MRC_TransRecv.MRC_online_flag)
            {
                OLED_ShowRssi(54,34,temp,16,0); 
            }
            else
            {
                OLED_ShowRssi(54,34,0,16,0);
            }
         break;
       case 2:
           if(SysCtrl_param.iap_flag == 1)
           {
             OLED_ShowString(60,25,"========",16,0);
             OLED_ShowChinese(128,25,"升级中",16,0);
             OLED_ShowString(180,25,"========",16,0);
             break;
           }
           OLED_ShowBattery_Power(236,15,AdcParam.bat_volt_filter,16,0);
           OLED_ShowString(132,15,":",16,0);
           OLED_ShowChinese(68,15,"控制模式",16,0);
           OLED_ShowString(132,15,":",16,0);
           switch(SysCtrl_param.temp_ctrl_rights)
           {
             case CTRL_RIGHT_NULL:
               OLED_ShowChinese(140,15,"无控制",16,0);
            break;                           
            case  CTRL_RIGHT_DOT:                
              OLED_ShowChinese(140,15,"手持终端点动",16,0);
            break;                            
            case  CTRL_RIGHT_HAND:               
              OLED_ShowChinese(140,15,"手持终端手动",16,0);
            break;                            
            case  CTRL_RIGHT_AUTO:               
              OLED_ShowChinese(140,15,"自动驾驶中",16,0);
            break; 
            case  CTRL_RIGHT_CONFIG:               
              OLED_ShowChinese(140,15,"配置中",16,0);
            break;
            case  CTRL_RIGHT_TASK:               
              OLED_ShowChinese(140,15,"任务中",16,0);
            break;
            case  CTRL_RIGHT_FRC_HAND:               
              OLED_ShowChinese(140,15,"遥控器手动",16,0);
            break;
            case  CTRL_RIGHT_FRC_DOT:               
              OLED_ShowChinese(140,15,"遥控器点动",16,0);
           break;    
            case  CTRL_RIGHT_WAIT:               
              OLED_ShowChinese(140,15,"等待中",16,0);              
             break;   
           }
       case 3: 
         break;
       default: break;
             
      }
   
    }
}

void OLED_Display_Refresh(void)
{
    if(OLED_Display_Status.EmergeStop != KeyParam.EmergeStop)
    {
        OLED_Fill(0,0,256,64,0x00);
        OLED_Display_Status.EmergeStop  = KeyParam.EmergeStop;
    }
//    if(OLED_Display_Status.OffLineFlg != OLED_Display_Status.OffLine)
//    {
//        OLED_Fill(0,0,256,64,0x00);
//        OLED_Display_Status.OffLineFlg = OLED_Display_Status.OffLine;
//    }
//    if(OLED_Display_Status.MRC_from_MCTRL_flag != MRC_TransRecv.MRC_online_flag)
//    {
//        OLED_Fill(0,0,256,64,0x00);
//        OLED_Display_Status.MRC_from_MCTRL_flag = MRC_TransRecv.MRC_online_flag;
//    }
    if(OLED_Display_Status.EmergeStopList != fromTCBD_data.msg1.EmergeStop)
    {
        OLED_Fill(0,0,256,64,0x00);
        OLED_Display_Status.EmergeStopList = fromTCBD_data.msg1.EmergeStop;
    }
//    if(OLED_Display_Status.errflag_num != fromTCBD_data.msg1.errflag_num)
//    {
//        OLED_Fill(0,0,256,64,0x00);
//        OLED_Display_Status.errflag_num  = fromTCBD_data.msg1.errflag_num;
//    }
     if(OLED_Display_Status.temp_ctrl_rights != SysCtrl_param.temp_ctrl_rights)
     {
        OLED_Fill(140,15,235,32,0x00);
        OLED_Display_Status.temp_ctrl_rights = SysCtrl_param.temp_ctrl_rights;
    }
    if(OLED_Display_Status.iap_flag != SysCtrl_param.iap_flag)
    {
        OLED_Fill(0,0,256,64,0x00);
        OLED_Display_Status.iap_flag = SysCtrl_param.iap_flag;
    }
}

void OledDisplayTest(void)
{
  OLED_Fill(0,0,256,64,0x00);
}

void OledDisplayTest2(void)
{
  uint8_t i;
#if 0  
    OLED_Fill(0,0,206,36,0);
    for(i=0;i<20;i++)
    {
       OLED_Fill(0,0,i*10,36,0xFF);
       delay_ms(1000);
    }
    OLED_Fill(0,0,206,36,0xFF);
    delay_ms(1000);
       
    OLED_Fill(0,0,206,36,0);
    
    for(i=0;i<18;i++)
    {
       OLED_Fill(0,0,256,i*2,0xff);
       delay_ms(1000);
    }
    OLED_Fill(0,0,206,36,0);    
#endif
		OLED_Fill(0,0,256,64,0);
    for(i=0;i<32;i++)
    {
       OLED_Fill(0,0,i*8,64,0xFF);
       delay_ms(1000);
    }
       
    OLED_Fill(5,5,250,60,0);
    
    for(i=0;i<16;i++)
    {
       OLED_Fill(0,0,256,i*4,0xff);
       delay_ms(1000);
    }
    OLED_Fill(0,0,256,64,0);    
    
}

//******************************************************************************

//    函数说明：OLED显示电压字符串

//******************************************************************************
void Show_BataVolt_Chinese(float voltage,u16 x,u16 y,u16 sizey,u8 mode)
{
  uint8_t temp_volt[6]={0,0,'.',0,'V'};
  uint16_t temp_data = (uint16_t)(voltage*10);

  temp_data = temp_data % 1000;
  
  temp_volt[0]=  temp_data/100;
  if(temp_volt[0]) temp_volt[0] += 0x30;
  else temp_volt[0] = ' ';
  
  temp_volt[1] = (temp_data % 100)/10 + 0x30;
  temp_volt[2] = '.';
  temp_volt[3] =(temp_data % 10) + 0x30;
  temp_volt[4] ='V';
  temp_volt[5] =0;
  
  OLED_ShowString( x, y,temp_volt, sizey, mode);
}

//******************************************************************************

//    函数说明：信号等级判断

//******************************************************************************
uint8_t  Get_Rssi_Level(uint32_t rssi)
{
    uint8_t rssi_level;
    
    if((rssi > 0)&&(rssi < 21))
    { 
      rssi_level = 6;
    }
    else if((rssi >= 21)&&(rssi < 41))
    { 
      rssi_level = 5;
    }
    else if((rssi >= 41)&&(rssi < 61)) 
    { 
      rssi_level = 4;
    }
    else if((rssi >= 61)&&(rssi < 71)) 
    { 
      rssi_level = 3;
    }
    else if((rssi >= 71)&&(rssi < 80)) 
    { 
      rssi_level = 2;
    }
    else if((rssi >= 81)&&(rssi < 98)) 
    { 
      rssi_level = 1;
    }
    else
    {
      rssi_level = 0;
    }
    return rssi_level;
}
//******************************************************************************

//    函数说明：电池电压等级判断

//******************************************************************************
uint8_t  Get_Power_Level(float bat_volt)
{
    float temp_volt;
    uint8_t volt_level = 0;

#if 1    
    float bat_volt_HtoL[10] = {12.0,11.80,11.50,11.30,11.10,11.00,10.90,10.70,10.50,9.00};
        
    temp_volt = (AdcParam.bat_volt_filter);
    
    
    for(uint8_t i=0;i<10;i++)
    {
        if(temp_volt > bat_volt_HtoL[i])
        {
            volt_level = 10- i;
            return volt_level;
        }    
    }    
    return volt_level;
#else    
  
    temp_volt = (AdcParam.bat_volt_filter);
    
    if(temp_volt > 12.00) 
    { 
      volt_level = 10;
    }
    else if ((temp_volt <= 12.00) && (temp_volt > 11.80))
    { 
      volt_level = 9; 
    }
    else if ((temp_volt <= 11.80) && (temp_volt > 11.50))
    { 
      volt_level = 8; 
    }
    else if ((temp_volt <= 11.50) && (temp_volt > 11.30))
    { 
      volt_level = 7; 
    }
    else if ((temp_volt <= 11.30) && (temp_volt > 11.10))
    { 
      volt_level = 6; 
    }
    else if ((temp_volt <= 11.10) && (temp_volt > 11.00))
    {
      volt_level = 5;
    }
    else if ((temp_volt <= 11.00) && (temp_volt > 10.90))
    { 
      volt_level = 4; 
    }
    else if ((temp_volt <= 10.90) && (temp_volt > 10.70))
    { 
      volt_level = 3; 
    }
    else if ((temp_volt <= 10.70) && (temp_volt > 10.50))
    { 
      volt_level = 2; 
    }
    else if ((temp_volt <= 10.50) && (temp_volt > 9.00))
    { 
      volt_level = 1;
    }
    else
    {
      volt_level = 0;
    }
    return volt_level;    
#endif
    

}
//******************************************************************************

//    函数说明：OLED显示电池图标

//******************************************************************************
void OLED_ShowBattery_Power(u16 x,u16 y,float battery,u16 sizey,u8 mode)
{
	u8 i,j,c,t=4,m,temp,DATA=0;
	u8 num;
	x/=2;
	c=Get_Power_Level(battery);
	num=(sizey/16+((sizey%16)?1:0))*sizey;
	Column_Address(x,x+sizey/4-1);
	Row_Address(y,y+sizey-1);
	for(i=0;i<num;i++)
	{
		if(sizey==16)temp=Battery_Power[c][i];//调用8x16字符

		else return;
		if(sizey%16)
		{
			m=sizey/16+1;
			if(i%m) t=2;
			else t=4;
		}
		for(j=0;j<t;j++)
		{
			if(temp&(0x01<<(j*2+0)))
			{
				DATA=0xf0;
			}
			if(temp&(0x01<<(j*2+1)))
			{
				DATA|=0x0f;
			}
			if(mode)
			{
				OLED_WR_Byte(~DATA);
			}else
			{
				OLED_WR_Byte(DATA);
			}
			DATA=0;
		}
	}
}
//******************************************************************************

//    函数说明：OLED显示信号图标,(列扫描正常竖显)

//******************************************************************************
void OLED_ShowRssi(u16 x,u16 y,uint32_t  rssi,u16 sizey,u8 mode)
{
	u8 i,j,c,t=4,m,temp,DATA=0;
	u8 num;
	x/=2;
	c=Get_Rssi_Level(rssi);
	num=(sizey/16+((sizey%16)?1:0))*sizey;
        Column_Address(x,x+sizey/4-1);
	Row_Address(y,y+sizey-1);
	for(i=0;i<num;i++)
	{
		if(sizey==16)temp=Lora_Rssi[c][i];//调用16x8字符

		else return;
		if(sizey%16)
		{
			m=sizey/16+1;
			if(i%m) t=2;
			else t=4;
		}
		for(j=0;j<t;j++)
		{
			if(temp&(0x01<<(j*2+0)))
			{
				DATA=0xf0;
			}
			if(temp&(0x01<<(j*2+1)))
			{
				DATA|=0x0f;
			}
			if(mode)
			{
				OLED_WR_Byte(~DATA);
			}else
			{
				OLED_WR_Byte(DATA);
			}
			DATA=0;
		}
	}
}



