#ifndef __TCP_H_
#define __TCP_H_
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <main.h>

#define ZQ_SERVER_IP "192.168.1.53"
#define ZQ_SERVER_PORT 7100

void Close_ZQ(void);
void Check_TCP_Connect(void);
void Send_TCP_DATA(char *pdata, uint16_t plen);

#endif
