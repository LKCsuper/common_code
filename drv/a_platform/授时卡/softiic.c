#include "softiic.h"

void softiic_start(softiic_t *iic)
{
    //  iic->clk(0);
    //  iic->delay();
    iic->dat(1);
    iic->delay();
    iic->clk(1);
    iic->delay();
    iic->dat(0);
    iic->delay();
    iic->clk(0);
    iic->delay();
}

void softiic_stop(softiic_t *iic)
{
    iic->dat(0);
    iic->delay();
    iic->clk(1);
    iic->delay();
    iic->dat(1);
    iic->delay();
    //  iic->dat(0);
    //  iic->delay();
    iic->clk(0);
    iic->delay();
}

void softiic_tack(softiic_t *iic)
{
    iic->clk(0);
    iic->delay();
    iic->dat(0);
    iic->delay();
    iic->clk(1);
    iic->delay();
    iic->clk(0);
    iic->delay();
}

void softiic_tnack(softiic_t *iic)
{
    iic->clk(0);
    iic->delay();
    iic->dat(1);
    iic->delay();
    iic->clk(1);
    iic->delay();
    iic->clk(0);
    iic->delay();
}

// NOTE 注意检测延时
int softiic_ckack(softiic_t *iic)
{
    uint16_t timeout = 0;

    iic->dat(1);
    iic->delay();
    iic->clk(1);
    iic->delay();
    while (iic->get_dat()) {
        timeout++;
        if (timeout >= 10) {
            iic->clk(0);
            iic->delay();
            return 1;
        }
    }
    iic->clk(0);
    iic->delay();

    return 0;
}

void softiic_write_byte(softiic_t *iic, uint8_t chByte)
{
    uint8_t i;

    for (i = 0; i < 8; i++) {
        if (chByte & 0x80) {
            iic->dat(1);
        } else {
            iic->dat(0);
        }
        iic->delay();
        iic->clk(1);
        iic->delay();
        iic->clk(0);
        iic->delay();
        chByte = chByte << 1;
    }
}

void softiic_read_byte(softiic_t *iic, uint8_t *pchByte)
{
    uint8_t i;

    for (i = 0; i < 8; i++) {
        iic->dat(1);
        iic->delay();
        iic->clk(1);
        iic->delay();
        *pchByte = *pchByte << 1;
        if (iic->get_dat()) {
            *pchByte = *pchByte | 0x01;
        }
        iic->delay();
        iic->clk(0);
        iic->delay();
    }
}

int softiic_read_buff(softiic_t *iic, uint8_t addr, uint16_t len, uint8_t *buff)
{
    int ret = 0;
    uint8_t chByte;

    softiic_start(iic);
    softiic_write_byte(iic, iic->dev_addr);
    ret |= softiic_ckack(iic);

    softiic_write_byte(iic, addr);
    ret |= softiic_ckack(iic);

    softiic_start(iic);
    softiic_write_byte(iic, iic->dev_addr | 0x01);
    ret |= softiic_ckack(iic);
    while (len) {
        softiic_read_byte(iic, &chByte);
        *buff = chByte;
        if (len == 1) {
            softiic_tnack(iic);
        } else {
            softiic_tack(iic);
        }
        buff++;
        len--;
    }
    softiic_stop(iic);

    return ret;
}

int softiic_write_buff(softiic_t *iic, uint8_t addr, uint16_t len, uint8_t *buff)
{
    int ret = 0;

    softiic_start(iic);
    softiic_write_byte(iic, iic->dev_addr);
    ret |= softiic_ckack(iic);

    softiic_write_byte(iic, addr);
    ret |= softiic_ckack(iic);

    while (len) {
        softiic_write_byte(iic, *buff);
        ret |= softiic_ckack(iic);
        buff++;
        len--;
    }
    softiic_stop(iic);

    return ret;
}

int softiic_write_bits(softiic_t *iic, uint8_t addr, uint8_t mask, uint8_t data)
{
    int ret = 0;
    uint8_t chByte;

    ret |= softiic_read_buff(iic, addr, 1, &chByte);
    chByte &= ~mask;
    chByte |= data;
    ret |= softiic_write_buff(iic, addr, 1, &chByte);

    return ret;
}

int softiic_read_bits(softiic_t *iic, uint8_t addr, uint8_t mask, uint8_t *data)
{
    int ret = 0;
    uint8_t chByte;

    ret |= softiic_read_buff(iic, addr, 1, &chByte);
    chByte &= mask;
    *data = chByte;

    return ret;
}

/**
 * @brief  IIC接口注册函数
 * @note
 * @param  *iic:
 * @param  (*clk:
 * @retval None
 */
void softiic_interface_register(softiic_t *iic, uint8_t dev_addr, void (*clk)(uint8_t chByte),
                                void (*dat)(uint8_t chByte), uint8_t (*get_dat)(void), void (*delay)(void))
{
    iic->clk = clk;
    iic->dat = dat;
    iic->get_dat = get_dat;
    iic->delay = delay;

    iic->dev_addr = dev_addr;
}
