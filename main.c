#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "r3.h"


double myWonderfulUnpacker(uint8_t *can_message, size_t message_size) {
    struct r3_group0_t rpm_data; // make struct 
    int result = r3_group0_unpack(&rpm_data, can_message, message_size); // give data to struct with the rpm sxtractor 
    if (result < 0) {
        fprintf(stderr, "Failed to unpack CAN message\n");
        return -1;
    }
    return r3_group0_rpm_decode(rpm_data.rpm); // decode the rpm
}


void simulateCANMessage(uint8_t *buffer) {
    static uint16_t raw_rpm = 0;
    raw_rpm = (raw_rpm + 250) % 8000;

    uint8_t high_byte = raw_rpm / 256;
    uint8_t low_byte  = raw_rpm % 256;

    buffer[0] = high_byte;
    buffer[1] = low_byte;

    for (int i = 2; i < 8; i++){
        buffer[i] = 0;
    } 
}


int main() {
    uint8_t can_data[8];

    while (true) {
        simulateCANMessage(can_data);
        double rpm = myWonderfulUnpacker(can_data, sizeof(can_data));
        if (rpm >= 0) {
            printf("RPM: %.2f\n", rpm);
        }
        usleep(200000);  // 0.2 second delay
    }
    return 0;
}