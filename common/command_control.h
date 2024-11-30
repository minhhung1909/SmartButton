#ifndef __COMMAND_CONTROL_H__
#define __COMMAND_CONTROL_H__

#define DATA_REC_OK               0x00
#define DATA_REC_ERROR            0xFF

#define TURN_ON_DEVICE      ((0x01 << 8) | 0x01)
#define TURN_OFF_DEVICE     ((0x02 << 8) | 0x02)
#define GET_STATE_DEVICE    ((0x03 << 8) | 0x03)
#define ALARM               ((0x04 << 8) | 0x04)

#endif // __COMMAND_CONTROL_H__