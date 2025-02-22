/*
 * @Description:
 * @Version: 1.0
 * @Author: lkc
 * @detail:
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "Lcd_Driver.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
// 从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
// 通过该函数转换
// c:GBR格式的颜色值
// 返回值：RGB格式的颜色值
uint16_t LCD_BGR2RGB(uint16_t c)
{
    uint16_t r, g, b, rgb;
    b = (c >> 0) & 0x1f;
    g = (c >> 5) & 0x3f;
    r = (c >> 11) & 0x1f;
    rgb = (b << 11) + (g << 5) + (r << 0);
    return (rgb);
}

// 画圆圈
void Gui_Circle(uint16_t X, uint16_t Y, uint16_t R, uint16_t fc)
{ // Bresenham算法
    unsigned short a, b;
    int c;
    a = 0;
    b = R;
    c = 3 - 2 * R;
    while (a < b) {
        Gui_DrawPoint(X + a, Y + b, fc); //        7
        Gui_DrawPoint(X - a, Y + b, fc); //        6
        Gui_DrawPoint(X + a, Y - b, fc); //        2
        Gui_DrawPoint(X - a, Y - b, fc); //        3
        Gui_DrawPoint(X + b, Y + a, fc); //        8
        Gui_DrawPoint(X - b, Y + a, fc); //        5
        Gui_DrawPoint(X + b, Y - a, fc); //        1
        Gui_DrawPoint(X - b, Y - a, fc); //        4

        if (c < 0) {
            c = c + 4 * a + 6;
        } else {
            c = c + 4 * (a - b) + 10;
            b -= 1;
        }
        a += 1;
    }
    if (a == b) {
        Gui_DrawPoint(X + a, Y + b, fc);
        Gui_DrawPoint(X + a, Y + b, fc);
        Gui_DrawPoint(X + a, Y - b, fc);
        Gui_DrawPoint(X - a, Y - b, fc);
        Gui_DrawPoint(X + b, Y + a, fc);
        Gui_DrawPoint(X - b, Y + a, fc);
        Gui_DrawPoint(X + b, Y - a, fc);
        Gui_DrawPoint(X - b, Y - a, fc);
    }
}

/**
 * @description: 画线函数，使用Bresenham 画线算法
 * @detail:
 * @param {uint16_t} x0
 * @param {uint16_t} y0
 * @param {uint16_t} x1
 * @param {uint16_t} y1
 * @param {uint16_t} Color
 * @return {*}
 * @author: lkc
 */
void Gui_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t Color)
{
    int dx,  // difference in x's
        dy,  // difference in y's
        dx2, // dx,dy * 2
        dy2,
        x_inc, // amount in pixel space to move during drawing
        y_inc, // amount in pixel space to move during drawing
        error, // the discriminant i.e. error i.e. decision variable
        index; // used for looping


    Lcd_SetXY(x0, y0);
    dx = x1 - x0; // 计算x距离
    dy = y1 - y0; // 计算y距离

    if (dx >= 0) {
        x_inc = 1;
    } else {
        x_inc = -1;
        dx = -dx;
    }

    if (dy >= 0) {
        y_inc = 1;
    } else {
        y_inc = -1;
        dy = -dy;
    }

    dx2 = dx << 1;
    dy2 = dy << 1;

    if (dx > dy) // x距离大于y距离，那么每个x轴上只有一个点，每个y轴上有若干个点
    {            // 且线的点数等于x距离，以x轴递增画点
        // initialize error term
        error = dy2 - dx;

        // draw the line
        for (index = 0; index <= dx; index++) // 要画的点数不会超过x距离
        {
            // 画点
            Gui_DrawPoint(x0, y0, Color);

            // test if error has overflowed
            if (error >= 0) // 是否需要增加y坐标值
            {
                error -= dx2;

                // move to next line
                y0 += y_inc; // 增加y坐标值
            } // end if error overflowed

            // adjust the error term
            error += dy2;

            // move to the next pixel
            x0 += x_inc; // x坐标值每次画点后都递增1
        } // end for
    } // end if |slope| <= 1
    else // y轴大于x轴，则每个y轴上只有一个点，x轴若干个点
    {    // 以y轴为递增画点
        // initialize error term
        error = dx2 - dy;

        // draw the line
        for (index = 0; index <= dy; index++) {
            // set the pixel
            Gui_DrawPoint(x0, y0, Color);

            // test if error overflowed
            if (error >= 0) {
                error -= dy2;

                // move to next line
                x0 += x_inc;
            } // end if error overflowed

            // adjust the error term
            error += dx2;

            // move to the next pixel
            y0 += y_inc;
        } // end for
    } // end else |slope| > 1
}

/**
 * @description: 画box
 * @detail:
 * @param {uint16_t} x
 * @param {uint16_t} y
 * @param {uint16_t} w
 * @param {uint16_t} h
 * @param {uint16_t} bc
 * @return {*}
 * @author: lkc
 */
void Gui_box(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t bc)
{
    Gui_DrawLine(x, y, x + w, y, 0xEF7D);
    Gui_DrawLine(x + w - 1, y + 1, x + w - 1, y + 1 + h, 0x2965);
    Gui_DrawLine(x, y + h, x + w, y + h, 0x2965);
    Gui_DrawLine(x, y, x, y + h, 0xEF7D);
    Gui_DrawLine(x + 1, y + 1, x + 1 + w - 2, y + 1 + h - 2, bc);
}

/**
 * @description: 画box2
 * @detail:
 * @param {uint16_t} x
 * @param {uint16_t} y
 * @param {uint16_t} w
 * @param {uint16_t} h
 * @param {uint8_t} mode
 * @return {*}
 * @author: lkc
 */
void Gui_box2(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t mode)
{
    if (mode == 0) {
        Gui_DrawLine(x, y, x + w, y, 0xEF7D);
        Gui_DrawLine(x + w - 1, y + 1, x + w - 1, y + 1 + h, 0x2965);
        Gui_DrawLine(x, y + h, x + w, y + h, 0x2965);
        Gui_DrawLine(x, y, x, y + h, 0xEF7D);
    }
    if (mode == 1) {
        Gui_DrawLine(x, y, x + w, y, 0x2965);
        Gui_DrawLine(x + w - 1, y + 1, x + w - 1, y + 1 + h, 0xEF7D);
        Gui_DrawLine(x, y + h, x + w, y + h, 0xEF7D);
        Gui_DrawLine(x, y, x, y + h, 0x2965);
    }
    if (mode == 2) {
        Gui_DrawLine(x, y, x + w, y, 0xffff);
        Gui_DrawLine(x + w - 1, y + 1, x + w - 1, y + 1 + h, 0xffff);
        Gui_DrawLine(x, y + h, x + w, y + h, 0xffff);
        Gui_DrawLine(x, y, x, y + h, 0xffff);
    }
}


/**************************************************************************************
功能描述: 在屏幕显示一凸起的按钮框
输    入: uint16_t x1,y1,x2,y2 按钮框左上角和右下角坐标
输    出: 无
**************************************************************************************/
void DisplayButtonDown(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    Gui_DrawLine(x1, y1, x2, y1, GRAY2);             // H
    Gui_DrawLine(x1 + 1, y1 + 1, x2, y1 + 1, GRAY1); // H
    Gui_DrawLine(x1, y1, x1, y2, GRAY2);             // V
    Gui_DrawLine(x1 + 1, y1 + 1, x1 + 1, y2, GRAY1); // V
    Gui_DrawLine(x1, y2, x2, y2, WHITE);             // H
    Gui_DrawLine(x2, y1, x2, y2, WHITE);             // V
}

/**************************************************************************************
功能描述: 在屏幕显示一凹下的按钮框
输    入: uint16_t x1,y1,x2,y2 按钮框左上角和右下角坐标
输    出: 无
**************************************************************************************/
void DisplayButtonUp(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    Gui_DrawLine(x1, y1, x2, y1, WHITE); // H
    Gui_DrawLine(x1, y1, x1, y2, WHITE); // V

    Gui_DrawLine(x1 + 1, y2 - 1, x2, y2 - 1, GRAY1); // H
    Gui_DrawLine(x1, y2, x2, y2, GRAY2);             // H
    Gui_DrawLine(x2 - 1, y1 + 1, x2 - 1, y2, GRAY1); // V
    Gui_DrawLine(x2, y1, x2, y2, GRAY2);             // V
}

/**
 * @description: 写字
 * @detail:
 * @param {uint16_t} x
 * @param {uint16_t} y
 * @param {uint16_t} fc
 * @param {uint16_t} bc
 * @param {uint8_t} *s
 * @return {*}
 * @author: lkc
 */
void Gui_DrawFont_GBK16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s)
{
    unsigned char i, j;
    unsigned short k, x0;
    x0 = x;

    while (*s) {
        if ((*s) < 128) {
            k = *s;
            if (k == 13) {
                x = x0;
                y += 16;
            } else {
                if (k > 32) {
                    k -= 32;
                } else {
                    k = 0;
                }

                for (i = 0; i < 16; i++) {
                    for (j = 0; j < 8; j++) {
                        if (asc16[k * 16 + i] & (0x80 >> j)) {
                            Gui_DrawPoint(x + j, y + i, fc);
                        } else {
                            if (fc != bc) {
                                Gui_DrawPoint(x + j, y + i, bc);
                            }
                        }
                    }
                }
                x += 8;
            }
            s++;
        }

        else {


            for (k = 0; k < hz16_num; k++) {
                if ((hz16[k].Index[0] == *(s)) && (hz16[k].Index[1] == *(s + 1))) {
                    for (i = 0; i < 16; i++) {
                        for (j = 0; j < 8; j++) {
                            if (hz16[k].Msk[i * 2] & (0x80 >> j)) {
                                Gui_DrawPoint(x + j, y + i, fc);
                            } else {
                                if (fc != bc) {
                                    Gui_DrawPoint(x + j, y + i, bc);
                                }
                            }
                        }
                        for (j = 0; j < 8; j++) {
                            if (hz16[k].Msk[i * 2 + 1] & (0x80 >> j)) {
                                Gui_DrawPoint(x + j + 8, y + i, fc);
                            } else {
                                if (fc != bc) {
                                    Gui_DrawPoint(x + j + 8, y + i, bc);
                                }
                            }
                        }
                    }
                }
            }
            s += 2;
            x += 16;
        }
    }
}

/**
 * @description: 写字
 * @detail:
 * @param {uint16_t} x
 * @param {uint16_t} y
 * @param {uint16_t} fc
 * @param {uint16_t} bc
 * @param {uint8_t} *s
 * @return {*}
 * @author: lkc
 */
void Gui_DrawFont_GBK24(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint8_t *s)
{
    unsigned char i, j;
    unsigned short k;

    while (*s) {
        if (*s < 0x80) {
            k = *s;
            if (k > 32) {
                k -= 32;
            } else {
                k = 0;
            }

            for (i = 0; i < 16; i++) {
                for (j = 0; j < 8; j++) {
                    if (asc16[k * 16 + i] & (0x80 >> j)) {
                        Gui_DrawPoint(x + j, y + i, fc);
                    } else {
                        if (fc != bc) {
                            Gui_DrawPoint(x + j, y + i, bc);
                        }
                    }
                }
            }
            s++;
            x += 8;
        } else {

            for (k = 0; k < hz24_num; k++) {
                if ((hz24[k].Index[0] == *(s)) && (hz24[k].Index[1] == *(s + 1))) {
                    for (i = 0; i < 24; i++) {
                        for (j = 0; j < 8; j++) {
                            if (hz24[k].Msk[i * 3] & (0x80 >> j)) {
                                Gui_DrawPoint(x + j, y + i, fc);
                            } else {
                                if (fc != bc) {
                                    Gui_DrawPoint(x + j, y + i, bc);
                                }
                            }
                        }
                        for (j = 0; j < 8; j++) {
                            if (hz24[k].Msk[i * 3 + 1] & (0x80 >> j)) {
                                Gui_DrawPoint(x + j + 8, y + i, fc);
                            } else {
                                if (fc != bc) {
                                    Gui_DrawPoint(x + j + 8, y + i, bc);
                                }
                            }
                        }
                        for (j = 0; j < 8; j++) {
                            if (hz24[k].Msk[i * 3 + 2] & (0x80 >> j)) {
                                Gui_DrawPoint(x + j + 16, y + i, fc);
                            } else {
                                if (fc != bc) {
                                    Gui_DrawPoint(x + j + 16, y + i, bc);
                                }
                            }
                        }
                    }
                }
            }
            s += 2;
            x += 24;
        }
    }
}

/**
 * @description: 写数字
 * @detail:
 * @param {uint16_t} x
 * @param {uint16_t} y
 * @param {uint16_t} fc
 * @param {uint16_t} bc
 * @param {uint16_t} num
 * @return {*}
 * @author: lkc
 */
void Gui_DrawFont_Num32(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint16_t num)
{
    unsigned char i, j, k, c;
    // lcd_text_any(x+94+i*42,y+34,32,32,0x7E8,0x0,sz32,knum[i]);
    //	w=w/8;

    for (i = 0; i < 32; i++) {
        for (j = 0; j < 4; j++) {
            c = *(sz32 + num * 32 * 4 + i * 4 + j);
            for (k = 0; k < 8; k++) {

                if (c & (0x80 >> k)) {
                    Gui_DrawPoint(x + j * 8 + k, y + i, fc);
                } else {
                    if (fc != bc) {
                        Gui_DrawPoint(x + j * 8 + k, y + i, bc);
                    }
                }
            }
        }
    }
}
#ifdef __cplusplus
}
#endif
