#ifndef __GPIO_H
#define __GPIO_H
/********************************************************************
Output  begin
*********************************************************************/
/*  4G RST=PB9 PWR=PB8 AUDIO_EN = PB7  PWR_4G_EN = PB6 */ 
#define G4_RST_PIN         GPIO_Pin_9
#define G4_RST_PRT         GPIOB

#define G4_PWRKEY_PIN      GPIO_Pin_8
#define G4_PWRKEY_PRT      GPIOB

#define G4_AUDIO_EN_PIN    GPIO_Pin_7
#define G4_AUDIO_EN_PRT    GPIOB

#define G4_PWR_EN_PIN      GPIO_Pin_6
#define G4_PWR_EN_PRT      GPIOB

#define G4_RST_EN()        GPIO_SetBits(G4_RST_PRT,G4_RST_PIN)
#define G4_RST_DIS()       GPIO_ResetBits(G4_RST_PRT,G4_RST_PIN)

#define G4_PWRKEY_EN()     GPIO_SetBits(G4_PWRKEY_PRT,G4_PWRKEY_PIN)
#define G4_PWRKEY_DIS()    GPIO_ResetBits(G4_PWRKEY_PRT,G4_PWRKEY_PIN)

#define G4_AUDIO_EN_EN()   GPIO_SetBits(G4_AUDIO_EN_PRT,G4_AUDIO_EN_PIN)
#define G4_AUDIO_EN_DIS()  GPIO_ResetBits(G4_AUDIO_EN_PRT,G4_AUDIO_EN_PIN)

#define G4_PWR_EN_EN()     GPIO_SetBits(G4_PWR_EN_PRT,G4_PWR_EN_PIN)
#define G4_PWR_EN_DIS()    GPIO_ResetBits(G4_PWR_EN_PRT,G4_PWR_EN_PIN)

/* hwt  */
#define HWT_PWR_EN_PIN     GPIO_Pin_2
#define HWT_PWR_EN_PRT     GPIOC

#define HWT_PWR_EN_EN()    GPIO_SetBits(HWT_PWR_EN_PRT, HWT_PWR_EN_PIN)
#define HWT_PWR_EN_DIS()   GPIO_ResetBits(HWT_PWR_EN_PRT, HWT_PWR_EN_PIN)

/*  Lora RST = PA6  LORA M0=PA4  M1=PA5 PWR_EN= PA7 */         
#define LORA_RST_PIN       GPIO_Pin_6
#define LORA_RST_PRT       GPIOA

#define LORA_M0_PIN        GPIO_Pin_4   
#define LORA_M0_PRT        GPIOA

#define LORA_M1_PIN        GPIO_Pin_5 
#define LORA_M1_PRT        GPIOA

#define LORA_PWR_PIN       GPIO_Pin_7 
#define LORA_PWR_PRT       GPIOA

#define LORA_RST_H()       GPIO_SetBits(LORA_RST_PRT,LORA_RST_PIN)
#define LORA_RST_L()       GPIO_ResetBits(LORA_RST_PRT,LORA_RST_PIN)

#define LORA_M0_H()        GPIO_SetBits(LORA_M0_PRT,LORA_M0_PIN) 
#define LORA_M0_L()        GPIO_ResetBits(LORA_M0_PRT,LORA_M0_PIN)

#define LORA_M1_H()        GPIO_SetBits(LORA_M1_PRT,LORA_M1_PIN) 
#define LORA_M1_L()        GPIO_ResetBits(LORA_M1_PRT,LORA_M1_PIN)

#define LORA_PWR_0N()      GPIO_ResetBits(LORA_PWR_PRT,LORA_PWR_PIN)
#define LORA_PWR_OFF()     GPIO_SetBits(LORA_PWR_PRT,LORA_PWR_PIN) 


/*  GPS PWR_EN = PC10  RST = PC4*/
#define GPS_RST_PIN        GPIO_Pin_4
#define GPS_RST_PRT        GPIOC

#define GPS_PWR_PIN        GPIO_Pin_10
#define GPS_PWR_PRT        GPIOC

#define GPS_RST_H()        GPIO_SetBits(GPS_RST_PRT,GPS_RST_PIN)
#define GPS_RST_L()        GPIO_ResetBits(GPS_RST_PRT,GPS_RST_PIN)

#define GPS_PWR_ON()       GPIO_SetBits(GPS_PWR_PRT,GPS_PWR_PIN)
#define GPS_PWR_OFF()      GPIO_ResetBits(GPS_PWR_PRT,GPS_PWR_PIN)

/* LED  LED1 = PC13  */    
#define LED1_PIN           GPIO_Pin_13
#define LED1_PRT           GPIOC

#define LED1_ON()          GPIO_SetBits(LED1_PRT,LED1_PIN)
#define LED1_OFF()         GPIO_ResetBits(LED1_PRT,LED1_PIN)
#define LED1_TOGG()        GPIO_ToggleBits(LED1_PRT,LED1_PIN)

/* SPI1 FLASH W25Q128 CS  PA15 */        
#define SPI1_CS_PIN        GPIO_Pin_15   
#define SPI1_CS_PRT        GPIOA

#define SPI1_CS_H()        GPIO_SetBits(SPI1_CS_PRT,SPI1_CS_PIN) 
#define SPI1_CS_L()        GPIO_ResetBits(SPI1_CS_PRT,SPI1_CS_PIN)

/* 电源开关 可以实现自关机 和按钮开机 PB12*/  
#define SELF_PWR_PIN       GPIO_Pin_12  
#define SELF_PWR_PRT       GPIOB

#define SELF_PWR_ON()      GPIO_SetBits(SELF_PWR_PRT,SELF_PWR_PIN) 
#define SELF_PWR_OFF()     GPIO_ResetBits(SELF_PWR_PRT,SELF_PWR_PIN)

/* 电源开关控制对外输出 RSV = PA12 CAMERA = PA11 LASER=PC8 
   LIGHT = PC9 BUZZER = PA8
*/ 
#define PWR_RSV_PIN        GPIO_Pin_12  
#define PWR_RSV_PRT        GPIOA

#define PWR_CAMERA_PIN     GPIO_Pin_11  
#define PWR_CAMERA_PRT     GPIOA

#define PWR_LASER_PIN      GPIO_Pin_8 
#define PWR_LASER_PRT      GPIOC

#define PWR_LIGHT_PIN      GPIO_Pin_9 
#define PWR_LIGHT_PRT      GPIOC

#define PWR_NANO_PIN      GPIO_Pin_14 
#define PWR_NANO_PRT      GPIOB

#define PWR_RSV_ON()       GPIO_SetBits(PWR_RSV_PRT,PWR_RSV_PIN) 
#define PWR_RSV_OFF()      GPIO_ResetBits(PWR_RSV_PRT,PWR_RSV_PIN)

#define PWR_CAMERA_ON()    GPIO_SetBits(PWR_CAMERA_PRT,PWR_CAMERA_PIN) 
#define PWR_CAMERA_OFF()   GPIO_ResetBits(PWR_CAMERA_PRT,PWR_CAMERA_PIN)

#define PWR_LASER_ON()     GPIO_SetBits(PWR_LASER_PRT,PWR_LASER_PIN) 
#define PWR_LASER_OFF()    GPIO_ResetBits(PWR_LASER_PRT,PWR_LASER_PIN)

#define PWR_LIGHT_ON()     GPIO_SetBits(PWR_LIGHT_PRT,PWR_LIGHT_PIN) 
#define PWR_LIGHT_OFF()    GPIO_ResetBits(PWR_LIGHT_PRT,PWR_LIGHT_PIN)

#define BUZZER_PIN         GPIO_Pin_8
#define BUZZER_PRT         GPIOA

// #define BUZZER_ON()        GPIO_ResetBits(BUZZER_PRT, BUZZER_PIN)
// #define BUZZER_OFF()       GPIO_SetBits(BUZZER_PRT, BUZZER_PIN)
#define BUZZER_ON()        GPIO_SetBits(BUZZER_PRT, BUZZER_PIN)
#define BUZZER_OFF()       GPIO_ResetBits(BUZZER_PRT, BUZZER_PIN)
#define BUZZER_TOG()       GPIO_ToggleBits(BUZZER_PRT, BUZZER_PIN)

/* 倾角计 电源开关 PWR_ANGLE_EN = PC2*/   
#define PWR_ANGLE_PIN      GPIO_Pin_8  
#define PWR_ANGLE_PRT      GPIOA

#define PWR_ANGLE_ON()     GPIO_SetBits(PWR_ANGLE_PRT,PWR_ANGLE_PIN) 
#define PWR_ANGLE_OFF()    GPIO_ResetBits(PWR_ANGLE_PRT,PWR_ANGLE_PIN)

#define PWR_NANO_ON()      GPIO_SetBits(PWR_NANO_PRT, PWR_NANO_PIN) 
#define PWR_NANO_OFF()     GPIO_ResetBits(PWR_NANO_PRT, PWR_NANO_PIN)
/********************************************************************
Output  end
*********************************************************************/
/********************************************************************

Input  begin
*********************************************************************/
#define KEY_ON   1
#define KEY_OFF  0

#define INPUT1_PIN GPIO_Pin_2
#define INPUT1_PRT GPIOE

#define INPUT2_PIN GPIO_Pin_3
#define INPUT2_PRT GPIOE

#define INPUT3_PIN GPIO_Pin_4
#define INPUT3_PRT GPIOE

#define INPUT4_PIN GPIO_Pin_5
#define INPUT4_PRT GPIOE

#define INPUT5_PIN GPIO_Pin_6
#define INPUT5_PRT GPIOA

#define INPUT6_PIN GPIO_Pin_7
#define INPUT6_PRT GPIOA

#define INPUT7_PIN GPIO_Pin_4
#define INPUT7_PRT GPIOC

#define INPUT8_PIN GPIO_Pin_5
#define INPUT8_PRT GPIOC

#define INPUT9_PIN GPIO_Pin_3
#define INPUT9_PRT GPIOD
/*
#define INPUT10_PIN GPIO_Pin_4
#define INPUT10_PRT GPIOD
*/
#define INPUT11_PIN GPIO_Pin_5
#define INPUT11_PRT GPIOD

#define INPUT12_PIN GPIO_Pin_6
#define INPUT12_PRT GPIOD

#define INPUT_LORA_AUX_PIN GPIO_Pin_3
#define INPUT_LORA_AUX_PRT GPIOC

#define GPIO_ReadInput1()  GPIO_ReadInputDataBit(INPUT1_PRT,INPUT1_PIN);
#define GPIO_ReadInput2()  GPIO_ReadInputDataBit(INPUT2_PRT,INPUT2_PIN);
#define GPIO_ReadInput3()  GPIO_ReadInputDataBit(INPUT3_PRT,INPUT3_PIN);
#define GPIO_ReadInput4()  GPIO_ReadInputDataBit(INPUT4_PRT,INPUT4_PIN);
#define GPIO_ReadInput5()  GPIO_ReadInputDataBit(INPUT5_PRT,INPUT5_PIN);
#define GPIO_ReadInput6()  GPIO_ReadInputDataBit(INPUT6_PRT,INPUT6_PIN);
#define GPIO_ReadInput7()  GPIO_ReadInputDataBit(INPUT7_PRT,INPUT7_PIN);
#define GPIO_ReadInput8()  GPIO_ReadInputDataBit(INPUT8_PRT,INPUT8_PIN);
#define GPIO_ReadInput9()  GPIO_ReadInputDataBit(INPUT9_PRT,INPUT9_PIN);
//#define GPIO_ReadInput10()  GPIO_ReadInputDataBit(INPUT10_PRT,INPUT10_PIN);
#define GPIO_ReadInput11()  GPIO_ReadInputDataBit(INPUT11_PRT,INPUT11_PIN);
#define GPIO_ReadInput12()  GPIO_ReadInputDataBit(INPUT12_PRT,INPUT12_PIN);
#define GPIO_ReadInput_LORA_AUX()  GPIO_ReadInputDataBit(INPUT_LORA_AUX_PRT, INPUT_LORA_AUX_PIN)
/********************************************************************
Input  end
*********************************************************************/




void GPIO_Configuration(void);
int GPIO_BOARD_Init(void);


#endif