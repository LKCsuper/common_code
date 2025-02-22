目前应用：

    创建信号量，主要是用于创建信号量，用于进程间互斥



linux 加载库的时候，会先调用该函数，

int load() __attribute__((constructor));

int deload() __attribute__((destructor));
