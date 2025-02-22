/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @detail: 
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "ringfifo.h"
#include <stddef.h>


static uint32_t fifo_get_free_size(_fifo_t *pfifo);
uint32_t fifo_get_occupy_size(_fifo_t *pfifo);


/**
  * @brief  注册一个fifo
  * @param  pfifo: fifo结构体指针
            pfifo_buf: fifo内存块
            size: 长度
  * @retval none
*/
void fifo_register(_fifo_t *pfifo, uint8_t *pfifo_buf, uint32_t size,
                   lock_fun lock, lock_fun unlock)
{
    pfifo->buf_size = size;         //注册的缓冲区的大小，这个大小可以小于接收数据数组的实际大小
    pfifo->buf  = pfifo_buf;        //注册的缓冲区的内存地址指针
    pfifo->pwrite = pfifo->buf;     //初始化的时候读写指针都指向缓冲区的起始头部
    pfifo->pread  = pfifo->buf;
    pfifo->occupy_size = 0;         //缓冲区的有效数据的大小
    pfifo->lock = lock;             //不用锁的话这个应该是null,这两个是函数指针，通过指向的函数执行上锁解锁功能
    pfifo->unlock = unlock;
}
 
/**
  * @brief  释放fifo
  * @param  pfifo: fifo结构体指针
  * @retval none
*/
void fifo_release(_fifo_t *pfifo)
{
    pfifo->buf_size = 0;
    pfifo->occupy_size = 0;
    pfifo->buf = NULL;
    pfifo->pwrite = 0;
    pfifo->pread = 0;
    pfifo->lock = NULL;
    pfifo->unlock = NULL;
}
 
/**
  * @brief  往fifo写数据
  * @param  pfifo: fifo结构体指针
            pbuf: 待写数据
            size: 待写数据大小
  * @retval 实际写大小
*/
uint32_t fifo_write(_fifo_t *pfifo, const uint8_t *pbuf, uint32_t size)
{
    uint32_t w_size= 0,free_size = 0;

    if ((size==0) || (pfifo==NULL) || (pbuf==NULL))   //写数据的时候必须指定一个已经注册的缓冲区，且传输数据数不为0
    {
        return 0;
    }

    /* 把加锁操作放在这里 */
    if (pfifo->lock != NULL)  //写之前加锁
        pfifo->lock();

    free_size = fifo_get_free_size(pfifo);    //通过这一步，能够确保   写数据永远无法对读数据进行 套圈！！！！！！！
    if(free_size == 0)       //获取缓冲区的剩余空间的大小，如果没有剩余的空间了，则无法写入，返回0
    {
        /* 这里增加解锁操作 */
        if (pfifo->unlock != NULL)
            pfifo->unlock();
        return 0;
    }

    if(free_size < size)     //如果缓冲区的   剩余空间大小 < 需写入的数据量   ，写入剩余空间大小的数据，多余的无法继续写入！！！！！！！ 
    {
        size = free_size;
    }
    w_size = size;

    /*  把加锁操作提到前面，并增加相关流程的解锁操作
    if (pfifo->lock != NULL)  //写之前加锁
        pfifo->lock();
    */

    while(w_size-- > 0)         //循环写入数据
    {
        *pfifo->pwrite++ = *pbuf++;
        if (pfifo->pwrite >= (pfifo->buf + pfifo->buf_size))    //如果缓冲区写满，从缓冲区头部开始继续写入
        {
            pfifo->pwrite = pfifo->buf;
        }
        pfifo->occupy_size++;     //每写入一个字节，有效数据长度+1
    }

    if (pfifo->unlock != NULL)  //写完后解锁退出
        pfifo->unlock();

    return size;                //返回写入的字节数
}
 

/* 	缓冲区与数据解析的流程tfk_li
	应该有三种方式
	1：通过中断接收数据，然后在中断中解析数据，这种方式比较简单，但是不适合大量数据的接收，因为中断中不能进行复杂的数据处理
	2：通过DMA接收数据，然后在空闲中断中解析数据，这种方式比较复杂，但是适合大量数据的接收，因为空闲中断中可以进行复杂的数据处理
	3：通过中断或者DMA接收数据，然后在主循环中解析数据，这种方式比较简单，但是不适合大量数据的接收，因为主循环中不能进行复杂的数据处理

	关于这三种方式的选择，应该根据实际的应用场景来选择，如果是大量数据的接收，那么就选择第二种方式，如果是少量数据的接收，那么就选择第一种或者第三种方式

	数据保证不丢包的思考
	1：
	这个函数如果在非常苛刻的条件下接收解析数据，还要做到不丢包，就要再添加全局数组，对收到的前半部分的正确数据放在新的全局数组中进行解析。
	全局数组 buf1[xxx] 使用这个库进行注册管理，调用fifo_read 函数后把收到的前半部分的正确数据放在新的
	全局数组 buf2[xxx] 中，然后应用程序再次调用fifo_read 函数后，把收到的后半部分的正确数据放在buf2中，再对整包的数据进行解析

	这是一种多层次缓冲区的思想，可以解决丢包的问题，但是会增加代码量，增加复杂度，增加内存占用，增加运行时间，增加功耗，增加成本，增加维护成本，增加bug的概率

	2：
	如果不采用1的方法，那么
	这个读取的函数需要扩展一种形式的，即读取完成后不立即操作读指针，而是根据解析数据的结果来决定是否操作读指针及操作多少 (变量x），这样可以避免数据丢失
	这样的话，就需要在调用完fifo_read函数后，再调用一个函数f(x)，来判断数据是否正确，如果正确，再操作读指针，如果不正确，不操作读指针

	3：
	2中提供的思想是一种解决方案，但是这个方案还是有问题，因为这样又引入了一个变量 x(操作指针的量，pread指针和occupy_size有效数据跟这个新的变量x有关) ，但是x是不安全的，可能还没等调用函数f(x)重新操作读指针pread和occupy_size有效数据，
	别的线程就通过正常的读写操作对 整个缓冲区 进行了读写操作，变量x不能保证是安全的，所以这个方案也不可行。

	4：
	为什么把加锁的操作提前了？
	如果不提前，获取到了有效数据的大小，赋值给了一个局部变量。但是此时没有加锁，别的线程可能会对缓冲区进行读写操作，这样就会导致局部变量的值不准确。
	提前以后，从获取值到使用值的过程中，别的线程无法对缓冲区进行读写操作，这样就保证了局部变量的值是准确的。
*/


/**
  * @brief  从fifo读数据
  * @param  pfifo: fifo结构体指针
            pbuf: 待读数据缓存，将数据读取到这里进行处理
            size: 待读数据大小
  * @retval 实际读大小
*/
uint32_t fifo_read(_fifo_t *pfifo, uint8_t *pbuf, uint32_t size)
{
    uint32_t r_size = 0,occupy_size = 0;
        
    if ((size==0) || (pfifo==NULL) || (pbuf==NULL))   //读数据时必须指定一个已经注册的循环缓冲区，且必须指定数据流向，读数据量不能小于0
    {
        return 0;
    }

    /* 把加锁操作放在这里 */
    if (pfifo->lock != NULL)                        //读之前先加锁
        pfifo->lock();

    occupy_size = fifo_get_occupy_size(pfifo);      //获取有效数据大小，这个值在向缓冲区写数据的时候递增
    if(occupy_size == 0)
    {
        /* 这里增加解锁操作 */
        if (pfifo->unlock != NULL)
            pfifo->unlock();
        return 0;
    }

    if(occupy_size < size)                          //如果缓冲区中的    有效数据量 < 要读的数据量   ，读出有效数据即可！！！   如果有效数据量  >   要读的数据量   ，只读我们size想读的数量即可！！！！
    {
        size = occupy_size;
    }

    /* 把加锁操作提到前面，并增加相关流程的解锁操作
    if (pfifo->lock != NULL)                        //读之前先加锁
        pfifo->lock();
    */

    r_size = size;
    while(r_size-- > 0)
    {
        *pbuf++ = *pfifo->pread++;
        if (pfifo->pread >= (pfifo->buf + pfifo->buf_size)) 
        {
            pfifo->pread = pfifo->buf;
        }
        pfifo->occupy_size--;                     //有效数据递减
    }
    if (pfifo->unlock != NULL)
        pfifo->unlock();
    return size;                                    //返回读取的数据量
}

/**
  * @brief  获取fifo空间大小
  * @param  pfifo: fifo结构体指针
  * @retval fifo大小
*/
uint32_t fifo_get_total_size(_fifo_t *pfifo)
{
    if (pfifo==NULL)
        return 0;
    
    return pfifo->buf_size;   //返回整个缓冲区的总大小
}
 
/**
  * @brief  获取fifo空闲空间大小
  * @param  pfifo: fifo结构体指针
  * @retval 空闲空间大小
*/
static uint32_t fifo_get_free_size(_fifo_t *pfifo)
{
    uint32_t size;
    
    if (pfifo==NULL)
        return 0;
        
    size = pfifo->buf_size - fifo_get_occupy_size(pfifo);

    return size;
}
 
/**
  * @brief  获取fifo已用空间大小
  * @param  pfifo: fifo结构体指针
  * @retval fifo已用大小
*/
uint32_t fifo_get_occupy_size(_fifo_t *pfifo)
{
    if (pfifo==NULL)
        return 0;

    return  pfifo->occupy_size;
}

