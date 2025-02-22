/*
 * @Description:
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2023-07-20 20:23:03
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-08-05 14:29:07
 * @detail:
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
float Bsp_Adc_GetVoltage(VOID)
{
    float voltage = 0;
    adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_9,
                               ADC_SAMPLETIME_239POINT5);   // ch参数为ADC通道:ADC_CHANNEL_x  x = 0...15
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL); // 使用软件触发

    // while(!adc_flag_get(ADC0, ADC_FLAG_EOC));			//等待转换结束
    //     adc_flag_clear(ADC0, ADC_FLAG_EOC);

    voltage = ((adc_regular_data_read(ADC0) & 0xFFFF) * (3.3f / (4096.0f * 24.9f / 124.9f)));

    return voltage;
}

/**
 * @description: adc外设初始化
 * @detail:
 * @return {*}
 * @author: lkc
 */
int Bsp_Adc_Init(VOID)
{
    /* enable GPIOA clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_ADC0);
    rcu_periph_clock_enable(RCU_AF); /* 这里AF就是指我需要用Alternate Function的意思，复用的功能的Clock要打开 */

    /* config ADC clock */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV4);
    /* config the GPIO as analog mode */
    gpio_init(GPIOB, GPIO_MODE_AIN, GPIO_OSPEED_MAX, GPIO_PIN_1); /* GPIO_MODE_AIN 配置为Analog In mode */
    gpio_pin_remap_config(GPIO_ADC0_ETRGREG_REMAP, ENABLE);

    /* 配置ADC时钟来源 这里使用APB2时钟的六分频 */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV2);

    /* 数据对齐方式 右对齐*/
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    /* 配置使用ADC规则通道,使用1个 */
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1U);
    /* 配置ADC触发方式:这里使用软件触发 */
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
    /*使能ADC软件触发 */
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
    /* ADC分辨率 12B */
    adc_resolution_config(ADC0, ADC_RESOLUTION_12B);
    /* ADC外部触发配置 */
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
    /* 使能ADC */
    adc_enable(ADC0);
    /* 这个延时是为了让ADC稳定*/
    rt_thread_delay(10);
    /* 进行ADC自校准 */
    adc_calibration_enable(ADC0);

    PRINTF("Bsp Adc Volate Init\r\n");

    return 0;
}
INIT_BOARD_EXPORT(Bsp_Adc_Init);
#ifdef __cplusplus
}
#endif
