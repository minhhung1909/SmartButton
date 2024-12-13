#ifndef __COMMAND_CONTROL_H__
#define __COMMAND_CONTROL_H__

#define DATA_REC_OK               0x00
#define DATA_REC_ERROR            0xFF

#define TURN_ON_DEVICE      ((uint8_t[]){0x01, 0x01})
#define TURN_OFF_DEVICE     ((uint8_t[]){0x02, 0x02})
#define GET_STATE_DEVICE    ((uint8_t[]){0x03, 0x03})
#define ALARM               ((uint8_t[]){0x04, 0x04})
#define FEED_BACK_CMD       ((uint8_t[]){0x05, 0x05})
#define FEED_BACK_TEST      ((uint8_t[]){0x06, 0x06})

#endif // __COMMAND_CONTROL_H__