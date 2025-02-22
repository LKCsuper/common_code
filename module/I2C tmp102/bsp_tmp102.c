#include "tmp102.h"
#include "stdio.h"
#include "string.h"


/*
 * 功能: I2C_0外设初始化
 * 参数：无
 * 返回值：无 
 */
void I2C_Config(void)
{
	  // enable the clk
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_I2C0, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOA, true);

    // set iomux
    gpio_set_iomux(GPIOA, GPIO_PIN_14, 3);
    gpio_set_iomux(GPIOA, GPIO_PIN_15, 3);

    // init
		i2c_config_t config;
		
		config.mode = I2C_MODE_MASTER;
		config.settings.master.speed = I2C_CR_BUS_MODE_STANDARD;
    i2c_config_init(&config);
    i2c_init(I2C0, &config);
    i2c_cmd(I2C0, true);
}

/* 功能: I2C_0读取单个字节数据
 * 参数 - data：读取的寄存器
 * 返回值：读到的字节
 */
void Tmp102_WriteDat(uint8_t reg, char *buf, int len)
{
    /* 开启写 */
    i2c_master_send_start(I2C0, SLAVE_ADDR, I2C_WRITE);
    i2c_clear_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY);
    while (i2c_get_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY) != SET);

	    /* 写操作地址 */
    i2c_send_data(I2C0, reg);
    i2c_clear_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY);
    while (i2c_get_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY) != SET);
	
    for(int i = 0; i < len; i++) {
        i2c_send_data(I2C0, buf[i]);
        i2c_clear_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY);
        while (i2c_get_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY) != SET);
    }

    i2c_master_send_stop(I2C0);

    return;
}

/* 功能: 获取TMP102温度数据
 * 参数 - reg:要读取的数据所在寄存器
 * 参数 - data：要读取的数据缓冲区
 * 参数 - cnt: 要读取的数据长度
 * 返回值：读到的字节
 */
void TMP102_ReadDat(uint8_t reg, char *buf, int len)
{
		int i = 0;
    /* 发送设备地址 */
    i2c_master_send_start(I2C0, SLAVE_ADDR, I2C_WRITE);
    i2c_clear_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY);
    while (i2c_get_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY) != SET);

    /* 写操作地址 */
    i2c_send_data(I2C0, reg);
    i2c_clear_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY);
    while (i2c_get_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY) != SET);

#if 0
    /* 写读地址 */
    i2c_send_data(I2C0, SLAVE_ADDR);
    i2c_send_data(I2C0, reg);
    i2c_clear_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY);
    while (i2c_get_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY) != SET);
#endif

    /* 重新启动i2c */
    i2c_master_send_start(I2C0, SLAVE_ADDR, I2C_READ);
    i2c_clear_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY);
    while (i2c_get_flag_status(I2C0, I2C_FLAG_TRANS_EMPTY) != SET);
		
    for(i=0; i<len-1; i++)
    {
        /* 读取数据 */
        i2c_set_receive_mode(I2C0, I2C_ACK);
        while (i2c_get_flag_status(I2C0, I2C_FLAG_RECV_FULL) != SET);
        i2c_clear_flag_status(I2C0, I2C_FLAG_RECV_FULL);
        buf[i] = i2c_receive_data(I2C0);
    }

    /* 读取数据 */
    i2c_set_receive_mode(I2C0, I2C_NAK);
    while (i2c_get_flag_status(I2C0, I2C_FLAG_RECV_FULL) != SET);
    i2c_clear_flag_status(I2C0, I2C_FLAG_RECV_FULL);
    buf[i] = i2c_receive_data(I2C0);

    /* 停止i2c */
    i2c_master_send_stop(I2C0);

    return;
}

void Tmp102_read(char *str)
{
	char buf[2];
	uint16_t dat = 0;
	float temp = 0.0;
	char strff[8];
	
	
	TMP102_ReadDat(TEMP_REG, buf, 2);
	
	dat = (buf[0] << 4)  | (buf[1] >> 4);
//	dat = ((buf[1] << 8)  | buf[2]) >> 4;
	if(dat & 0x800)
	{
			buf[0] = '-';
			dat = ~dat;
			temp = (dat + 1 ) * 0.0625;
			temp = -temp;
	} else {
			temp = dat * 0.0625;
	}
	
	memset(strff, 0, sizeof(strff));
	sprintf(strff,"%.2f", temp);
	
	memcpy(str, strff, sizeof(strff));
	printf("temp: %s\r\n", strff);
}