#ifndef __APP_TCP_CLIENT_H__
#define __APP_TCP_CLIENT_H__

extern uint8_t g_command[2];
extern uint8_t g_data_length[2];
extern uint8_t g_data[256];
extern int g_size_data;

void tcp_client(char host_ip[], int port);

#endif