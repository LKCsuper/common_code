/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @detail: 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct DHT11_SENSOR_DATA
{
    u16 temp; // Temperature
    u16 hum;  // Humidity
} dht11_data;

volatile sig_atomic_t exit_flag = 0;

void sigint_handler(int signo)
{
    if (signo == SIGINT)
    {
        exit_flag = 1;
    }
}

int main(void)
{
    int fd;
    int retval;
    dht11_data Curdht11_data;

    signal(SIGINT, sigint_handler);
	printf("Press CTRL+C to exit.\n");

    fd = open("/dev/dht11", O_RDONLY);
    if (fd == -1)
    {
        perror("open dht11 error\n");
        exit(-1);
    }

	sleep(1);
    printf("open /dev/dht11 successfully\n");

    while (!exit_flag)
    {
        sleep(1);
        retval = read(fd, &Curdht11_data, sizeof(Curdht11_data));
        if (retval == -1)
        {
            perror("read dht11 error");
            printf("read dht11 error");
            exit(-1);
        }
        if (Curdht11_data.temp != 0xffff)
            printf("Temperature:%d.%d C, Humidity:%d.%d %%RH\n", Curdht11_data.temp >> 8, Curdht11_data.temp & 0xff, \
                                                                    Curdht11_data.hum >> 8, Curdht11_data.hum & 0xff);
    }

    close(fd);
	sleep(1);
    return 0;
}
