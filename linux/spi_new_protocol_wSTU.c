#include <stdint.h>  
#include <unistd.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <getopt.h>  
#include <fcntl.h>  
#include <sys/ioctl.h>  
#include <linux/types.h>  
#include <linux/spi/spidev.h>  
#include <time.h>
  
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))  
  
static void pabort(const char *s)  
{  
    perror(s);  
    abort();  
}  
  
static const char *device = "/dev/spidev0.0";  
static uint8_t mode = SPI_CPHA | SPI_CPOL; //SPI_MODE3
static uint8_t bits = 8;  
static uint32_t speed = 1000000;   // max speed = 1000000
static uint16_t delay = 0;
clock_t sum_clock = 0;

static void transfer(int fd, char* tx, int len)  
{  
    int ret;  
    uint8_t rx[256];  
    for(int i =0 ; i < 256 ; i ++){
        rx[i] = 0;
    }

    struct spi_ioc_transfer tr[] = {  
        {
            .tx_buf = (unsigned long)tx,  
            .rx_buf = (unsigned long)rx,  
            .len = len,  
            .delay_usecs = delay,  
            .speed_hz = speed,  
            .bits_per_word = bits,  
        }
    };  
 
    clock_t start = clock();
    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    sum_clock += clock() - start;
    if (ret < 1)  
        pabort("can't send spi message");  
 
    for (ret = 0; ret < len; ret++)   
        printf("%d ", rx[ret]);  


}  
  
  
int main()
{  
    int ret = 0;  
    int fd;  
  
  
    fd = open(device, O_RDWR);  
    if (fd < 0)  
        pabort("can't open device");  
  
    /* 
     * spi mode 
     */  
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);  
    if (ret == -1)  
        pabort("can't set spi mode");  
  
    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);  
    if (ret == -1)  
        pabort("can't get spi mode");  
  
    /* 
     * bits per word 
     */  
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);  
    if (ret == -1)  
        pabort("can't set bits per word");  
  
    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);  
    if (ret == -1)  
        pabort("can't get bits per word");  
  
    /* 
     * max speed hz 
     */  
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);  
    if (ret == -1)  
        pabort("can't set max speed hz");  
  
    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);  
    if (ret == -1)  
        pabort("can't get max speed hz");  
  
    printf("spi mode: %d\n", mode);  
    printf("bits per word: %d\n", bits);  
    printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);  
 
    const int buf_length = 256;
    char buf[buf_length];

    // Header
    buf[0] = 0x81;
    buf[1] = 0x82;
    buf[2] = 0x83;
    //buf[3] ~ buf[255] : data
    for(int i =3 ; i < 256 ; i++){
        buf[i] = 0;
    }
    transfer(fd, buf, 256);

    //Reauesting return value
    buf[0] = 0x84;
    buf[1] = 0x85;
    buf[2] = 0x86;
    transfer(fd, buf, 3);

    puts("");
    printf("spi transfer delay time (micro second) : %f\n", (double)sum_clock / CLOCKS_PER_SEC *1000 * 1000);

    close(fd);  
  
    return ret;  
}
