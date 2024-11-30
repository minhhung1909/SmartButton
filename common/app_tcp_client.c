#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>            // struct addrinfo
#include <arpa/inet.h>
#include "esp_netif.h"
#include "esp_log.h"
#include "app_tcp_client.h"
#include "frame_crc.h"

static const char *TAG_TCP = "tcp ";
static const char *payload = " Message from ESP32 connect OK";

static const char *payload_OK = "DATA OK";
static const char *payload_ERROR = "DATA ERROR";

uint8_t g_command[2] = {'x','x'};
uint8_t g_data_length[2] = {0}; 
uint8_t g_data[256] = {0};
int g_size_data = 0;

void tcp_client(char host_ip[], int port) {
    char rx_buffer[128] = {};
    int addr_family = 0;
    int ip_protocol = 0;

    while (1) {
        struct sockaddr_in dest_addr;
        inet_pton(AF_INET, host_ip, &dest_addr.sin_addr);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(port);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;

        int sock =  socket(addr_family, SOCK_STREAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG_TCP, "Unable to create socket: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG_TCP, "Socket created, connecting to %s:%d", host_ip, port);

        int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err != 0) {
            ESP_LOGE(TAG_TCP, "Socket unable to connect: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG_TCP, "Successfully connected");
        
        err = send(sock, payload, strlen(payload), 0); 
        if (err < 0) {
            ESP_LOGE(TAG_TCP, "Error occurred during sending: errno %d", errno);
            break;
        }

        while (1) {
            int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
            if (len < 0) {
                ESP_LOGE(TAG_TCP, "recv failed: errno %d", errno);
                break;
            }
            else {
                rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
                if(check_crc((unsigned char*)rx_buffer) == 1){
                    ESP_LOGI(TAG_TCP, "Received %d bytes from %s:", len, host_ip);
                    ESP_LOGI(TAG_TCP, "%s", rx_buffer);

                    err = send(sock, payload_OK, strlen(payload_OK), 0);
                    if (err < 0) {
                        ESP_LOGE(TAG_TCP, "Error occurred during sending: errno %d", errno);
                        break;
                    }

                    memcpy(g_command, rx_buffer + 3, 2);
                    memcpy(g_data_length, rx_buffer + 5, 2);

                    g_size_data = (g_data_length[0] << 8) | g_data_length[1];
                    memcpy(g_data, rx_buffer + 7, g_size_data);
                                
                }else{
                    ESP_LOGE(TAG_TCP, "CRC check failed.");
                    err = send(sock, payload_ERROR, strlen(payload_ERROR), 0);
                    if (err < 0) {
                        ESP_LOGE(TAG_TCP, "Error occurred during sending: errno %d", errno);
                        break;
                    }
                }
            }
        }

        if (sock != -1) {
            ESP_LOGE(TAG_TCP, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
}
