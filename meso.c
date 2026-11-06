#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define CS_PIN = 'P9_17'
#define CLK_PIN = 'P9_22'
#define MOSI_PIN = 'P9_18'
#define MISO_PIN = 'P9_21'
#define SPI_DEVICE "/dev/spidev1.0"


int main(){
    int fd = open(SPI_DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open SPI device");
        return -1;
    }
    uint8_t mode = SPI_MODE_0;
    uint8_t bits = 8;
    uint32_t speed = 500000;
    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) {
        perror("Failed to set SPI mode");
        close(fd);
        return -1;
    }
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) {
        perror("Failed to set bits per word");
        close(fd);
        return -1;
    }
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        perror("Failed to set max speed");
        close(fd);
        return -1;
    }
    uint8_t tx_buffer[5]="nigga";
    uint8_t rx_buffer[sizeof(tx_buffer)] = {0};
    struct spi_ioc_transfer transfer = {
        .tx_buf = (unsigned long)tx_buffer,
        .rx_buf = (unsigned long)rx_buffer,
        .len = sizeof(tx_buffer),
        .speed_hz = speed,
        .bits_per_word = bits,
        .delay_usecs = 0,
    };
    ioctl(fd, SPI_IOC_MESSAGE(1), &transfer);
    printf(
        "Received: %s\n", rx_buffer
    );
    close(fd);
    return 0;
}