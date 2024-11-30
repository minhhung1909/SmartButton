#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "frame_crc.h"
#include "command_control.h"

uint16_t calculate_crc16(const uint8_t *data, size_t length) {
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xA001; 
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

// Function for packet
uint8_t *build_packet(const uint8_t *command, const uint8_t *data, size_t data_length, size_t *frame_length) {
    uint8_t header[3] = {0x54, 0x48, 0x55};
    uint8_t stop_byte[1] = {0x2A};

    size_t packet_size = sizeof(header) + 2 + 2 + data_length;  // Header (3) + Command (2) + Data Length (2) + Data (X)
    *frame_length = packet_size + 2 + 1;                            // addition 2 byte crc + stop byte
    uint8_t *packet = (uint8_t *)malloc(*frame_length);

    if (!packet) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    size_t offset = 0;

    // Header
    memcpy(packet + offset, header, 3);
    offset += 3;

    // Command
    memcpy(packet + offset, command, 2);
    offset += 2;

    // Data length (2 byte)
    uint8_t data_len[2] = {(uint8_t)(data_length >> 8), (uint8_t)(data_length & 0xFF)};
    memcpy(packet + offset, data_len, 2);
    offset += 2;

    // Data
    memcpy(packet + offset, data, data_length);
    offset += data_length;

    // cal CRC
    uint16_t crc = calculate_crc16(packet, offset);
    packet[offset++] = crc & 0xFF;       
    packet[offset++] = (crc >> 8) & 0xFF;

    memcpy(packet + offset, stop_byte, 1);
    return packet;
}

int check_crc(const uint8_t *frame) {
    uint8_t stop_byte = 0x2A;

    size_t frame_length = 0;
    while(frame[frame_length] != stop_byte ) {
        frame_length++;
    }
    frame_length++;

    size_t data_length = frame_length - 3;
    uint16_t calculated_crc = calculate_crc16(frame, data_length);
    uint16_t received_crc = frame[frame_length - 3] | (frame[frame_length - 2] << 8);

    if (calculated_crc != received_crc) {
        return -1;
    }
    return 1;
}

size_t get_Length_Frame(const uint8_t *frame) {
    size_t frame_length = 0;
    while(frame[frame_length] != 0x2A ) {
        frame_length++;
    }
    return ++frame_length;
}

/* EXAMPLE */ 
/* DEMO: 54 48 55 01 01 00 03 01 02 03 A0 67 2A
int main() {
    uint8_t command[] = TURN_ON_DEVICE;
    uint8_t data[3] = {0x01, 0x02, 0x03};
    size_t frame_length = 0;

    uint8_t *frame = build_packet(command, data, sizeof(data), &frame_length);
    if (frame) {
        printf("Generated packet: ");
        for (size_t i = 0; i < frame_length; i++) {
            printf("%02X ", frame[i]);
        }
        // free(frame);
    }
    printf("\n");
    printf("Frame length main: %d\n", frame_length);

    size_t data_length_new = get_Length_Frame(frame);
    printf("Data length new: %d\n", data_length_new);

    uint8_t frame_copy[data_length_new];
    for(int i = 0 ; i < data_length_new; i++) {
        frame_copy[i] = frame[i];
    }

    for(int i = 0; i < data_length_new; i++) {
        printf("%02X ", frame_copy[i]);
    }

    if (check_crc(frame_copy) == 1) {
        printf("\n CRC check passed.\n");
    } else {
        printf("\n CRC check failed.\n");
    }

    return 0;
}
*/