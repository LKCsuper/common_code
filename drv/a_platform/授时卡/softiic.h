#ifndef _SOFT_IIC_H_
#define _SOFT_IIC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct {
    uint8_t dev_addr;

    // 配置clk dat 端口操作接口
    void (*clk)(uint8_t chByte);
    void (*dat)(uint8_t chByte);
    uint8_t (*get_dat)(void);

    void (*delay)(void); // 延时函数
} softiic_t;

extern void softiic_interface_register(softiic_t *iic, uint8_t dev_addr, void (*clk)(uint8_t chByte),
                                       void (*dat)(uint8_t chByte), uint8_t (*get_dat)(void), void (*delay)(void));

extern void softiic_start(softiic_t *iic);
extern void softiic_stop(softiic_t *iic);
extern void softiic_tack(softiic_t *iic);
extern void softiic_tnack(softiic_t *iic);
extern int softiic_ckack(softiic_t *iic);

extern void softiic_write_byte(softiic_t *iic, uint8_t chByte);
extern void softiic_read_byte(softiic_t *iic, uint8_t *pchByte);

extern int softiic_write_buff(softiic_t *iic, uint8_t addr, uint16_t len, uint8_t *buff);
extern int softiic_read_buff(softiic_t *iic, uint8_t addr, uint16_t len, uint8_t *buff);

extern int softiic_write_bits(softiic_t *iic, uint8_t addr, uint8_t mask, uint8_t data);
extern int softiic_read_bits(softiic_t *iic, uint8_t addr, uint8_t mask, uint8_t *data);

#ifdef __cplusplus
}
#endif

#endif
