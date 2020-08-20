#ifndef __LED_H_
#define __LED_H_
#include <Arduino.h>
#include <Ticker.h>
#include <main.h>

#define LED_SYSTEM D2  //蓝色LED

void LED_init(void);
void Set_LED_mode(ZQ_STATUS_t zst,uint32_t ledms);

#endif
