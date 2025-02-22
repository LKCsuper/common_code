头文件：

#include <pthread.h>

makefile: -lpthread


函数原型：

int  pthread_create（（pthread_t  *thread,  pthread_attr_t  *attr,  void  *（*start_routine）（void  *）,  void  *arg）



getpid();//Linux 环境编程中用于定义进程 ID

pthread_self();//获取线程ID号

int pthread_join(pthread_t thread, void **value_ptr); // 一个线程等待另一个线程结束

void pthread_exit(void* retval); // 结束线程

int pthread_detach(pthread_t tid); // 主线程与子线程分离，资源在线程函数退出时或pthread_exit时自动会被释放

int pthread_cancel(pthread_t thread) // 从其他线程接收信号，终止该进程

int pthread_kill(pthread_t threadId,int signal); // 向同一进程下的另一线程发送信号，信号为0时用于检查此线程ID的线程是否存活,如果线程没有处理。默认杀死整个进程

pthread_equal // 检查两个线程ID是否相等

pthread_cleanup_push、pthread_cleanup_pop 线程退出，清理函数注册和执行



第一个参数为指向线程标识符的指针。

> - 当创建线程成功，线程ID由第一个参数返回。

- 第二个参数用来设置线程属性attr
- 第三个参数是线程运行函数的起始地址。
  - 第三个参数是线程的入口地址，线程是一个执行路线， 是进程中的单独控制序列，控制序列的入口地址由第三个参数指定。第三个参数是一个函数指针，函数指针所指向的函数特征如下，接收一个无类型的参数，返回的也是无类型指针。
- 第四个参数是运行函数的参数。
  - 第四个参数为传递给线程函数的参数，也就是第三个参数所指向函数的参数。尽管arg是**void ***类型的变量，但它同样可以作为任意类型的参数传给start_routine()函数；同时，**start_routine()**可以返回一个void *类型的返回值，而这个返回值也可以是其他类型，并由**pthread_join()**获取。
  - 如果需要向**start_routine**函数传递的参数不止一个，那么需要把这些参数放到一个结构中，然后把这个结构的地址作为**arg**的参数传入。


线程回调函数

//接着在线程的调用函数thr_fn中可以通过下面的方式使用通过para传入的参数。
void *thr_fn(void *arg)
{
       return ((void *)0);
}
