pthread_t t_id;
// 打印线程id
void printf_tid_pid(const char *s)
{
    pid_t pid;
    pthread_t tid;
    pid = getpid();       // Linux 环境编程中用于定义进程 ID
    tid = pthread_self(); // 获取线程ID号
    printf("%s pid %u tid %u (0x%x)\n", s, (unsigned)pid, (unsigned)tid, (unsigned)tid);
}

// 创建线程
int main()
{ 
    int arg = 10;
    pthread_t threadId;
    pthread_create(&threadId, NULL, func, &arg);
    while (1)
        ; // 让主线程不退出
    return 0;
}