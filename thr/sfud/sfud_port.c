/*
 * This file is part of the Serial Flash Universal Driver Library.
 *
 * Copyright (c) 2016-2018, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for each platform.
 * Created on: 2016-04-23
 */

// #include <sfud.h>
// #include <stdarg.h>
//#include "includes.h"
#include "stdio.h"
#include "rtthread.h"

#include "stm32f4xx.h"

#include "common.h"
#include "sfud.h"

#include "iwdg.h"
#include "gpio.h"
#include "spi.h"
#include "iwdg.h"

#include "w25qxx.h"

static char log_buf[256];

void sfud_log_debug(const char *file, const long line, const char *format, ...);

/**
 * SPI write data then read data
 */
static sfud_err spi_write_read(const sfud_spi *spi, const uint8_t *write_buf, size_t write_size, uint8_t *read_buf,
        size_t read_size) {
    sfud_err result = SFUD_SUCCESS;
    uint32_t i = 0;

    /* ι�� */
    IWDG_Feed();

    if (write_size) {
        SFUD_ASSERT(write_buf);
    }
    if (read_size) {
        SFUD_ASSERT(read_buf);
    }
    W25QXX_CS_L();                            //ʹ������   
    if (write_size)
    {
        for (i = 0; i < write_size; i++)
        {
            W25QXX_ReadWriteByte(write_buf[i]);
        }
    }
    if (read_size)
    {
        for (i = 0; i < read_size; i++)
        {
            read_buf[i] = W25QXX_ReadWriteByte(0xA5);
        }
    }
    W25QXX_CS_H();

    return result;
}

#ifdef SFUD_USING_QSPI
/**
 * read flash data by QSPI
 */
static sfud_err qspi_read(const struct __sfud_spi *spi, uint32_t addr, sfud_qspi_read_cmd_format *qspi_read_cmd_format,
        uint8_t *read_buf, size_t read_size) {
    sfud_err result = SFUD_SUCCESS;

    /**
     * add your qspi read flash data code
     */

    return result;
}
#endif /* SFUD_USING_QSPI */

static rt_mutex_t sfud_mutex = RT_NULL;
static void spi_lock(const sfud_spi *spi) {
    rt_mutex_take(sfud_mutex, RT_WAITING_FOREVER);
}

static void spi_unlock(const sfud_spi *spi) {
    rt_mutex_release(sfud_mutex);
}

static void retry_delay_100us(void) {
    uint32_t delay = 1000 * 5;
    while(delay--);
}

sfud_err sfud_spi_port_init(sfud_flash *flash) {
    sfud_err result = SFUD_SUCCESS;

    /**
     * add your port spi bus and device object initialize code like this:
     * 1. rcc initialize
     * 2. gpio initialize
     * 3. spi device initialize
     * 4. flash->spi and flash->retry item initialize
     *    flash->spi.wr = spi_write_read; //Required
     *    flash->spi.qspi_read = qspi_read; //Required when QSPI mode enable
     *    flash->spi.lock = spi_lock;
     *    flash->spi.unlock = spi_unlock;
     *    flash->spi.user_data = &spix;
     *    flash->retry.delay = null;
     *    flash->retry.times = 10000; //Required
     */

    /* 申�?�一�?互斥�? */
    sfud_mutex = rt_mutex_create("sfud_mutex", RT_IPC_FLAG_PRIO);

    if (RT_NULL == sfud_mutex)
    {
        rt_kprintf("create sfud_mutex mutex failed.\n");
        return SFUD_ERR_NOT_FOUND;
    }

    flash->spi.wr = spi_write_read;
    flash->spi.lock = spi_lock;
    flash->spi.unlock = spi_unlock;
    flash->retry.delay = retry_delay_100us;
    flash->retry.times = 10000;

    return result;
}

/**
 * This function is print debug info.
 *
 * @param file the file which has call this function
 * @param line the line number which has call this function
 * @param format output format
 * @param ... args
 */
void sfud_log_debug(const char *file, const long line, const char *format, ...) {
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    rt_kprintf("[SFUD](%s:%ld) ", file, line);
    /* must use vprintf to print */
    rt_vsnprintf(log_buf, sizeof(log_buf), format, args);
    rt_kprintf("%s\n", log_buf);
    va_end(args);
}

/**
 * This function is print routine info.
 *
 * @param format output format
 * @param ... args
 */
void sfud_log_info(const char *format, ...) {
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    rt_kprintf("[SFUD]");
    /* must use vprintf to print */
    rt_vsnprintf(log_buf, sizeof(log_buf), format, args);
    rt_kprintf("%s\n", log_buf);
    va_end(args);
}
