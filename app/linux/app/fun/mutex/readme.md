注意：这里其实都只是对于线程之间的互斥操作，但是实际上客户可能会多个线程调用，所以使用信号量更好一点





互斥锁

操作函数：

pthread_mutex_t lock; /* 互斥锁定义 */
pthread_mutex_init(&lock, NULL); /* 动态初始化，	成功返回0，失败返回非0 */
pthread_mutex_t thread_mutex = PTHREAD_MUTEX_INITIALIZER； /* 静态初始化，**通过这个功能可以避免使用init函数**  */
pthread_mutex_lock(&lock); /* 阻塞的锁定互斥锁 */
pthread_mutex_trylock(&thread_mutex)；/* 非阻塞的锁定互斥锁，成功获得互斥锁返回0，如果未能获得互斥锁，立即返回一个错误码 */
pthread_mutex_unlock(&lock)； /* 解锁互斥锁 */
pthread_mutex_destroy(&lock) /* 销毁互斥锁 */

#include <pthread.h>

// 互斥锁的声明和初始化
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// 锁定互斥锁
pthread_mutex_lock(&mutex);

// 执行对共享资源的操作

// 解锁互斥锁
pthread_mutex_unlock(&mutex);

// 销毁互斥锁
pthread_mutex_destroy(&mutex);
