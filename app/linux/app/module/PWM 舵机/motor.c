#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include "motor.h"
/* Private define ------------------------------------------------------------*/
#define PWM_PATH          "/sys/class/pwm/pwmchip10"
#define PWM_NUM           0
#define PERIOD_NS         1000000
#define MIN_DUTY_CYCLE_NS 0
#define MAX_DUTY_CYCLE_NS 1000000
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
pthread_t motor_thread;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
// 设置占空比
int motor_set_duty_cycle(uint64_t duty_cycle_ns)
{
    char duty_cycle_str[100];
    sprintf(duty_cycle_str, "%s/pwm%d/duty_cycle", PWM_PATH, PWM_NUM);
    FILE *duty_cycle_file = fopen(duty_cycle_str, "w");
    if (!duty_cycle_file) {
        perror("Failed to open PWM duty cycle");
        return 1;
    }
    fprintf(duty_cycle_file, "%llu", duty_cycle_ns);
    fclose(duty_cycle_file);

    return 0;
}

// 设置周期
int motor_set_period(uint64_t period_ns)
{
    char period_str[100];
    sprintf(period_str, "%s/pwm%d/period", PWM_PATH, PWM_NUM);
    FILE *period_file = fopen(period_str, "w");
    if (!period_file) {
        printf("Failed to open PWM period\r\n");
        perror("Failed to open PWM period");
        return 1;
    }
    fprintf(period_file, "%llu", period_ns);
    fclose(period_file);

    return 0;
}

// 使能
int motor_set_enable(int enable)
{
    char enable_str[100];
    sprintf(enable_str, "%s/pwm%d/enable", PWM_PATH, PWM_NUM);
    FILE *enable_file = fopen(enable_str, "w");
    if (!enable_file) {
        printf("Failed to open PWM enable\r\n");
        perror("Failed to open PWM enable");
        return 1;
    }
    fprintf(enable_file, "%d", enable);
    fclose(enable_file);

    return 0;
}   

// 设置极性
int motor_set_polarity(char *polarity)
{
    char polarity_str[100];
    sprintf(polarity_str, "%s/pwm%d/polarity", PWM_PATH, PWM_NUM);
    FILE *polarity_file = fopen(polarity_str, "w");
    if (!polarity_file) {
        printf("Failed to open PWM polarity\r\n");
        perror("Failed to open PWM polarity");
        return 1;
    }
    fprintf(polarity_file, "%s", polarity);
    fclose(polarity_file);

    return 0;
}

// 卸载
int motor_unexport(void)
{
    char unexport_str[100];
    sprintf(unexport_str, "%s/unexport", PWM_PATH);
    FILE *pwm_unexport = fopen(unexport_str, "w");
    if (!pwm_unexport) {
        printf("Failed to open PWM unexport\r\n");
        perror("Failed to open PWM unexport");
        return 1;
    }
    fprintf(pwm_unexport, "%d", PWM_NUM);
    fclose(pwm_unexport);

    return 0;
}

int motor_export(void)
{
    char export_str[100];
    sprintf(export_str, "%s/export", PWM_PATH);
    FILE *pwm_export = fopen(export_str, "w");
    if (!pwm_export) {
        printf("Failed to open PWM export\r\n");
        perror("Failed to open PWM export");
        return 1;
    }
    fprintf(pwm_export, "%d", PWM_NUM);
    fclose(pwm_export);

    return 0;
}

void *motor_thread_func(void *arg)
{
    printf("motor_thread_func\r\n");

    while (1)
    {
        // printf("motor_thread_func: motor_set_angle(0)\r\n");    
        motor_set_angle(0);
        sleep(1);
        // printf("motor_thread_func: motor_set_angle(45)\r\n");
        motor_set_angle(45);
        sleep(1);
        // printf("motor_thread_func: motor_set_angle(90)\r\n");
        motor_set_angle(90);
        sleep(1);
        // printf("motor_thread_func: motor_set_angle(135)\r\n");
        motor_set_angle(135);
        sleep(1);
        // printf("motor_thread_func: motor_set_angle(180)\r\n");
        motor_set_angle(180);
        sleep(1);
    }
    return NULL;
}

// 初始化
int motor_init(void)
{
    int ret;

    printf("motor_init\r\n");
    ret =   motor_export();
    if (ret != 0)
    {
        printf("motor_init: motor_export failed\r\n");
        // return ret;
    }
    printf("motor_export\r\n");
    ret = motor_set_period(20000000)    ;
    if (ret != 0)
    {
        printf("motor_init: motor_set_period failed\r\n");
        return ret;
    }
    printf("motor_set_period\r\n");
    ret = motor_set_enable(1);
    if (ret != 0)
    {
        printf("motor_init: motor_set_enable failed\r\n");
        return ret;
    }   
    printf("motor_set_enable\r\n");
    ret = motor_set_polarity("normal");
    if (ret != 0)
    {
        printf("motor_init: motor_set_polarity failed\r\n");
        return ret;
    }   
    printf("motor_set_polarity\r\n");

    printf("motor_set_angle\r\n");
    ret = motor_set_angle(0);
    if (ret != 0)
    {
        printf("motor_init: motor_set_angle failed\r\n");
        return ret;
    }
    printf("motor_set_angle\r\n");

    ret = pthread_create(&motor_thread, NULL, motor_thread_func, NULL);
    if (ret != 0)
    {
        printf("motor_init: pthread_create failed\r\n");
        return ret; 
    }

    return 0;
}

int motor_set_angle(int angle)
{
    int ret;

    switch (angle)
    {
    case 0:
        ret = motor_set_duty_cycle(500000);
        break;

    case 45:
        ret = motor_set_duty_cycle(1000000);
        break;

    case 90:
        ret = motor_set_duty_cycle(1500000);
        break;

    case 135:
        ret = motor_set_duty_cycle(2000000);
        break;

    case 180:
        ret = motor_set_duty_cycle(2500000);
        break;
    
    default:
        break;
    }

    return ret;
}

#ifdef __cplusplus
}
#endif