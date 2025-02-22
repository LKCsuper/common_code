/*****************************************************************************
*	模块名称： gpio.c                                         	
*	程序功能：                             
*	编 写 者：                                   
*	编写日期：
                    
* 说明： 

GPIO 配置标准：
1、管脚配置 从左上角开始配置，顺序执行各个管脚的配置；
2、管脚分组，对应一个原理图的GPIO，做一个注释说明
3、同一个文件里，从上向下，逐个配置；

**************************************************************************/  

#include "stm32f4xx.h"

#include "gpio.h"

void GPIO_output_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|
                           RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|
                           RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG, ENABLE);  
                   
 /*   all the same GPO config */   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   
    
/*  4G RST=PB9 PWR=PB8 AUDIO_EN = PB7  PWR_4G_EN = PB6 */ 
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9| GPIO_Pin_8 | GPIO_Pin_7| GPIO_Pin_6 ;   
     GPIO_Init(GPIOB, &GPIO_InitStructure);

/*  Lora RST = PA6  LORA M0=PA4  M1=PA5 PWR_EN= PA7 */       
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;   
     GPIO_Init(GPIOA, &GPIO_InitStructure);

/*  GPS PWR_EN = PC10  RST = PC4*/       
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10| GPIO_Pin_4;   
     GPIO_Init(GPIOC, &GPIO_InitStructure);     

/* LED  LED1 = PC13  */    
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;  
     GPIO_Init(GPIOC, &GPIO_InitStructure);       

/* SPI1 FLASH W25Q128 CS  PC11 */       
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;   
     GPIO_Init(GPIOC, &GPIO_InitStructure);     
     
/* 电源开关 可以实现自关机 和按钮开机 PB12*/     
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;   
     GPIO_Init(GPIOB, &GPIO_InitStructure);  

/* nano 开关 */
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;   
     GPIO_Init(GPIOB, &GPIO_InitStructure);  
/* 电源开关控制对外输出 RSV = PA12 CAMERA = PA11 LASER=PC8 
   LIGHT = PC9 BUZZER = PA8
*/   
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_8;   
     GPIO_Init(GPIOA, &GPIO_InitStructure);   

     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9;   
     GPIO_Init(GPIOC, &GPIO_InitStructure);   

 /* 倾角计 电源开关 PWR_ANGLE_EN = PC2*/       
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
     GPIO_Init(GPIOC, &GPIO_InitStructure);     
    
}


void GPIO_input_Configuration(void)
{

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|
                           RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|
                           RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG, ENABLE);  
    
/* 统一的GPI 输入配置，各个端口都一样 */     
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;  

/* LORA_AUX   =PC3 */    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOC, &GPIO_InitStructure);     

/* GPS_PPS   =PB0 */        
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;    
    GPIO_Init(GPIOB, &GPIO_InitStructure);   
}

void GPIO_interrupt_Configuration(void)
{

  
}

void GPIO_output_Init(void)
{
     /* 因为easyflash 外设初始化第一次时间太长, 可能会导致上电的命令错过 */
#if 0
    GPS_RST_H();
    GPS_PWR_ON();

    LORA_PWR_0N();

    LED1_ON(); 

    G4_PWRKEY_EN();
    G4_PWR_EN_EN();

    BUZZER_OFF();
#endif

    return;
}

void GPIO_Configuration(void)
{
    GPIO_output_Configuration();
    GPIO_output_Init();
    GPIO_input_Configuration();
    GPIO_interrupt_Configuration();
}

int GPIO_BOARD_Init(void)
{
     GPIO_Configuration();

     return 0;
}






