#ifndef __FRAME_CRC_H__
#define __FRAME_CRC_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

// Funtion for build packet
/*
parameter:
    - uint8_t command: command to send
    - uint8_t data: data to send
    - size_t data_length: length of data ( sizeof(data) )
    - frame_length: length of frame
return:
    - uint8_t *: frame
*/
uint8_t *build_packet(const uint8_t *command, const uint8_t *data, size_t data_length, size_t *frame_length);



// Function for check packet
/*
parameter:
    - uint8_t *frame: frame to check
return:
    - int: 1 if CRC is correct, -1 if CRC is incorrect
*/
int check_crc(const uint8_t *frame);



// Function for get length of frame
/*
parameter:
    - uint8_t *frame: frame to get length
return:
    - size_t: length of frame
*/
size_t get_Length_Frame(const uint8_t *frame);


uint8_t get_command(char rx_buffer[]);
#endif // __FRAME_CRC_H__