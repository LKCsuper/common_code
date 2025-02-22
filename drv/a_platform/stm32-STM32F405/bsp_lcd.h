
#ifndef __BSP_LCD_H
#define __BSP_LCD_H

#include "stm32f4xx.h"

typedef struct{
  
  int8_t    Left_UD_gear;
  int8_t    Left_LR_gear;
  int8_t    Right_UD_gear;
  
  uint8_t   lock;
  uint8_t   step;
  uint8_t   stop;  
  
  uint8_t   flag;
}struct_LcdParam;

extern struct_LcdParam LcdParam;


typedef struct{
   uint16_t  pos_x;
   uint16_t  pos_y;
   uint16_t  char_size; // 16 24 32
   uint8_t   charClass; //
	 uint8_t   max;
}struct_menu_oneParam;


#define  MENU_PAGE_MAX  2
#define SUB1_PARAM_MAX  10
#define SUB2_PARAM_MAX  10
#define SUB3_PARAM_MAX  10
#define  LCD_TIMEOUT_MS 10000

#define MAX_PREINSTALL_PAGE    4

typedef struct{
   uint8_t  livePos;
   uint8_t  lastPos;
   uint8_t  paran_cnt; // 16 24 32
}struct_menu_oneSub;

typedef struct{
  uint8_t    page_num;//25.5V
  uint32_t   timeout_ms;
  struct_menu_oneSub sub1;
  struct_menu_oneSub sub2;	
  struct_menu_oneSub sub3;		

}struct_Lcd_menu;
extern struct_Lcd_menu  Lcd_menu;

typedef struct{
  uint32_t       Waiver_Time;     //��Ȩ����ʱ
  uint32_t       Waiver_Second;   //��Ȩ��ʱ---ת��Ϊ��
  uint32_t       Sleep_Time;      //˯�ߵ���ʱ
  uint32_t       Sleep_Second;    //˯�߼�ʱ---ת��Ϊ��
  
  uint8_t        Preempt_Point;   //ң��Ȩ��ʼ��ռ��־
  
  uint8_t Line_length;     //����
  uint8_t Free_barrier_interval;      //���ϰ�����
  
  uint8_t Battery_Power;        //��ص���
  
  uint8_t LKnob_Data;   //�����ť��ֵ
  uint8_t RKnob_Data;     //�Ҳ���ť��ֵ
  
  uint8_t Location_Point;      //���е�ǰλ�ñ�־λ
  
  uint8_t LR_Point;       //���ҵ㶯��־λ
  uint8_t UD_Point;       //���µ㶯��־λ
  uint8_t FR_Point;       //ǰ��㶯��־λ
  uint8_t Last_LR_Point;    //�ϴ����ҵ㶯��־λ
  uint8_t Last_UD_Point;    //�ϴ����µ㶯��־λ
  uint8_t Last_FR_Point;    //�ϴ�ǰ��㶯��־λ
  
}struct_Global_Variable;

typedef struct{
  int8_t Key_Point;     //��ǰ����ֵ
  int8_t Clear_Show;     //��һ�ΰ���ֵ
  uint8_t Key_Press;     //�������±�־λ
  uint8_t Enter_First;     //��һ��ȷ��
  uint8_t Enter_Second;     //�ڶ���ȷ��
  uint8_t Last_Page;       //�ϴ�Ԥ�õ���ʾ����
  
  uint8_t Test_1;
  uint8_t Test_2;
  uint8_t Test_3;
  uint32_t Test_Time_1;
  uint32_t Test_Time_2;
  uint32_t Test_Time_3;
}struct_LCDP;
//------�쳣״̬��ʾ------//
typedef struct{
uint8_t EmergeStop;
uint8_t EmergeStopList;
uint8_t OffLine;
uint8_t OffLineFlg;
uint8_t DevOffLine;
uint8_t OffLineCount;
uint8_t OffLineTemp;
uint8_t MRC_from_MCTRL_flag;
uint8_t errflag_num;
uint8_t temp_ctrl_rights;
uint8_t iap_flag;
}struct_OLED_Display_Status;

extern struct_OLED_Display_Status  OLED_Display_Status;
//------�쳣״̬��ʾ------//
extern struct_Global_Variable Global_Variable;
extern struct_LCDP LCDP;

extern unsigned  char  Battery_Power[11][16];
extern unsigned  char  Lora_Rssi[7][16];
uint8_t  Get_Power_Level(float bat_volt);
void OLED_ShowBattery_Power(u16 x,u16 y,float battery,u16 sizey,u8 mode);
void OLED_ShowRssi(u16 x,u16 y,uint32_t  rssi,u16 sizey,u8 mode);
uint8_t  Get_Rssi_Level(uint32_t rssi);
void TestLCD_picture(void);

void Menu_Sub1_update(void);
void LCD_monitor(uint32_t time);

void LCD_displayCurrentDate(void);
void Main_Page (void);
void Preinstall_Page (void);
void Menu_Page (void);
void Safe_High_Page (void);
void Initialize_LCD_Data (void);
void Change_Video_Show (void);
void Show_Progress_Bar (uint8_t device ,int8_t power,uint32_t colour);
void new_page (void);
void Show_Preset_Point (uint8_t page);

void Global_Data (void);

void Direction_Key_Function (void);
void Set_Safe_High_Page (void);


void OLED_display_update(void);
void OledDisplayTest(void);
void OLED_Display_Refresh(void);

#endif

