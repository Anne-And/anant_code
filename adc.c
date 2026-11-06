#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ADC_PATH "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define MAX_BUFFER 64
#define REF_VOLTAGE 1.8
#define MAX_ADC_VALUE 4095.0

//P9_34 is the pin for AGND
//P9_39 is the pin for AIN0

double read_adc_voltage(const char *adc_file_path) {
    FILE *fp;
    char buffer[MAX_BUFFER];
    int raw_value = 0;

    fp = fopen(adc_file_path, "r");
    if (fp == NULL) {
        perror("Error opening ADC file");
        return -1.0;
    }

    if (fgets(buffer, MAX_BUFFER, fp) != NULL) {
        raw_value = atoi(buffer);
    } else {
        perror("Error reading ADC value");
        fclose(fp);
        return -1.0;
    }

    fclose(fp);

    // Convert to voltage
    return (raw_value / MAX_ADC_VALUE) * REF_VOLTAGE;
}

int main() {
    while (1) {
        double voltage = read_adc_voltage(ADC_PATH);
        if (voltage >= 0.0) {
            printf("ADC Reading: %.4f V\n", voltage);
        } else {
            fprintf(stderr, "Failed to read ADC value.\n");
        }

        usleep(500000);  // Sleep for 500ms
    }

    return 0;
}
