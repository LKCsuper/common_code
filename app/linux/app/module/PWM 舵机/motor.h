/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @detail: 
 */
#ifndef __MOTOR_H__
#define __MOTOR_H__
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
int motor_init(void);
int motor_set_duty_cycle(uint64_t duty_cycle_ns);
int motor_set_polarity(char *polarity);
int motor_set_period(uint64_t period_ns);
int motor_set_enable(int enable);
int motor_unexport(void);
int motor_set_angle(int angle);
#ifdef __cplusplus
}
#endif
#endif