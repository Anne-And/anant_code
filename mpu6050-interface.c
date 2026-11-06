#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <math.h>
#include <string.h>

#define I2C_BUS "/dev/i2c-2"  
#define MPU6050_ADDR 0x68
#define PWR_MGMT_1 0x6B
#define ACCEL_XOUT_H 0x3B
#define GYRO_XOUT_H  0x43
#define ACCEL_SCALE 16384.0   // LSB/g for ±2g
#define GYRO_SCALE 131.072    // LSB/(°/s) for ±250°/s

int i2c_read_register(int fd, uint8_t reg) {
    if (write(fd, &reg, 1) != 1) {
        perror("Failed to write register address");
        return -1;
    }
    uint8_t data;
    if (read(fd, &data, 1) != 1) {
        perror("Failed to read data");
        return -1;
    }
    return data;
}

int main() {
    int fd = open(I2C_BUS, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the I2C bus");
        return EXIT_FAILURE;
    }

    if (ioctl(fd, I2C_SLAVE, MPU6050_ADDR) < 0) {
        perror("Failed to connect to the MPU6050");
        close(fd);
        return EXIT_FAILURE;
    }

    // Wake up MPU6050
    uint8_t buffer[2] = {PWR_MGMT_1, 0x00};
    if (write(fd, buffer, 2) != 2) {
        perror("Failed to write to PWR_MGMT_1");
        close(fd);
        return EXIT_FAILURE;
    }

    usleep(10000); // Wait 10ms for sensor to stabilize

    while (1) {
        // Read accelerometer data
        int highx = i2c_read_register(fd, ACCEL_XOUT_H);
        int lowx  = i2c_read_register(fd, ACCEL_XOUT_H + 1);
        int highy = i2c_read_register(fd, ACCEL_XOUT_H + 2);
        int lowy  = i2c_read_register(fd, ACCEL_XOUT_H + 3);
        int highz = i2c_read_register(fd, ACCEL_XOUT_H + 4);
        int lowz  = i2c_read_register(fd, ACCEL_XOUT_H + 5);

        // Read gyroscope data
        int highxg = i2c_read_register(fd, GYRO_XOUT_H);
        int lowxg  = i2c_read_register(fd, GYRO_XOUT_H + 1);
        int highyg = i2c_read_register(fd, GYRO_XOUT_H + 2);
        int lowyg  = i2c_read_register(fd, GYRO_XOUT_H + 3);
        int highzg = i2c_read_register(fd, GYRO_XOUT_H + 4);
        int lowzg  = i2c_read_register(fd, GYRO_XOUT_H + 5);

        // Validate reads
        if (highx < 0 || lowx < 0 || highy < 0 || lowy < 0 ||
            highz < 0 || lowz < 0 || highxg < 0 || lowxg < 0 ||
            highyg < 0 || lowyg < 0 || highzg < 0 || lowzg < 0) {
            fprintf(stderr, "Error reading sensor data\n");
            break;
        }

        // Combine high and low bytes
        int16_t raw_ax = (int16_t)((highx << 8) | lowx);
        int16_t raw_ay = (int16_t)((highy << 8) | lowy);
        int16_t raw_az = (int16_t)((highz << 8) | lowz);

        int16_t raw_gx = (int16_t)((highxg << 8) | lowxg);
        int16_t raw_gy = (int16_t)((highyg << 8) | lowyg);
        int16_t raw_gz = (int16_t)((highzg << 8) | lowzg);

        // Convert to real units
        double accel_x = raw_ax / ACCEL_SCALE;
        double accel_y = raw_ay / ACCEL_SCALE;
        double accel_z = raw_az / ACCEL_SCALE;

        double gyro_x = raw_gx / GYRO_SCALE;
        double gyro_y = raw_gy / GYRO_SCALE;
        double gyro_z = raw_gz / GYRO_SCALE;

        // Display the results
        printf("Accelerometer (g): X=%.2f, Y=%.2f, Z=%.2f\n", accel_x, accel_y, accel_z);
        printf("Gyroscope (°/s):   X=%.2f, Y=%.2f, Z=%.2f\n", gyro_x, gyro_y, gyro_z);
        printf("------------------------------------------------\n");

        sleep(1);
    }

    close(fd);
    return EXIT_SUCCESS;
}
